#include "BioBlocksJSONReader.h"

using json = nlohmann::json;

BioBlocksJSONReader::BioBlocksJSONReader(const std::string & path, long timeSlice) throw(std::invalid_argument)
{
    this->tautology = std::make_shared<Tautology>();
    this->timeSlice = timeSlice;
    this->time = std::make_shared<VariableEntry>(TIME_VARIABLE);
    this->protocol = std::make_shared<ProtocolGraph>("translatedGraph");
    loadFile(path);
}

BioBlocksJSONReader::~BioBlocksJSONReader()
{
}

void BioBlocksJSONReader::loadFile(const std::string & path) throw(std::invalid_argument)
{
    std::ifstream in(path);
    json js;

    containerMap.clear();
    lastOp.clear();
    table_sequence.reset();
    graph_sequence.reset();
    protocol->setStartNode(0);
    finishTime = 0;
    operationMap.clear();

    try {
        in >> js;
        readRefs(js);

        readInstructions(js);
        parseHeap();

        LOG(INFO) << "translation done! id for container at the protocol:";
        for (auto it = containerMap.begin(); it != containerMap.end(); ++it) {
            LOG(INFO) << it->first << ", " << it->second;
        }
    } catch (invalid_argument & e) {
        throw(e);
    } catch (exception & e) {
        throw(std::invalid_argument("unknow exception: " + string(e.what())));
    }
}

std::shared_ptr<ProtocolGraph> BioBlocksJSONReader::getProtocol() {
    return protocol;
}

void BioBlocksJSONReader::readRefs(const nlohmann::json & js) throw(std::invalid_argument)
{
    json jrefs = js["refs"];

    for (json::iterator it = jrefs.begin(); it != jrefs.end(); ++it) {
        std::string name = (*it)["id"];
        boost::algorithm::trim(name);
        std::string volume_str = (*it)["volume"];
        float volume;
        patch::stod(volume, volume_str.c_str());

        auto entry = containerMap.find(name);
        if (entry == containerMap.end()) {
            int containerID = table_sequence.getNextValue();
            containerMap.insert(std::make_pair(name, containerID));

            std::shared_ptr<MathematicOperable> volumeObject = std::make_shared<ConstantNumber>(volume);
            std::shared_ptr<OperationNode> actual_op = std::make_shared<LoadContainerOperation>(graph_sequence.getNextValue(), containerID, volumeObject);
            protocol->addOperation(actual_op);

            if (!lastOp.empty()) {
                for (auto itop = lastOp.begin(); itop != lastOp.end(); ++itop) {
                    protocol->connectOperation(*itop, actual_op, tautology);
                }
                lastOp.clear();
            }
            lastOp.push_back(actual_op);
        } else {
            throw (invalid_argument("two containers has the same name: " + (*it)["id"].get<string>()));
        }
    }
}

void BioBlocksJSONReader::readInstructions(const nlohmann::json & js) {
    json jrefs = js["instructions"];
    for (json::iterator it = jrefs.begin(); it != jrefs.end(); ++it) {
        string op = (*it)["op"].get<string>();
        if (op.compare("pipette") == 0) {
            parsePipette(it);
        }
    }
}

void BioBlocksJSONReader::parseHeap() {
    if (!operationMap.empty()) {
        parseHeap_createInternalVariables();

        std::shared_ptr<ComparisonOperable> lastComp;
        std::shared_ptr<OperationNode> loop1 = parseHeap_initLoop(lastComp);

        int blockNum = 0;
        std::shared_ptr<OperationNode> lastIfStart;
        std::shared_ptr<OperationNode> lastIfStop;
        std::shared_ptr<ComparisonOperable> lastStartComp;
        std::shared_ptr<ComparisonOperable> lastStopComp;

        for (auto it = operationMap.begin(); it != operationMap.end(); ++it) {
            parseHeap_processOpBlock(blockNum,
                                     std::get<0>(it->first),
                                     std::get<1>(it->first),
                                     it->second,
                                     lastIfStart,
                                     lastStartComp,
                                     lastIfStop,
                                     lastStopComp,
                                     lastComp);
            blockNum++;
        }

        ProtocolGraph::ProtocolNodePtr timeStep = std::make_shared<TimeStep>(graph_sequence.getNextValue(), time);
        protocol->addOperation(timeStep);

        //connect last op
        for (std::shared_ptr<OperationNode> op: lastOp) {
            protocol->connectOperation(op, timeStep, lastComp);
        }
        lastOp.clear();

        //connect last ifStart no branch
        if (lastIfStart && lastStartComp) {
            protocol->connectOperation(lastIfStart, timeStep, lastStartComp);
        }

        //connect last ifStop no branch
        if (lastIfStop && lastStopComp) {
            protocol->connectOperation(lastIfStop, timeStep, lastStopComp);
        }

        protocol->connectOperation(timeStep, loop1, tautology);
    }
}

ProtocolGraph::ProtocolNodePtr BioBlocksJSONReader::parseHeap_initLoop(std::shared_ptr<ComparisonOperable> & lastComp) {
    std::shared_ptr<MathematicOperable> numTimeSlice = std::shared_ptr<MathematicOperable>(new ConstantNumber(timeSlice));
    ProtocolGraph::ProtocolNodePtr timeSliceNode = std::shared_ptr<OperationNode>(new SetTimeStep(graph_sequence.getNextValue(), numTimeSlice));

    protocol->addOperation(timeSliceNode);
    if (!lastOp.empty()) {
        for (auto itop = lastOp.begin(); itop != lastOp.end(); ++itop) {
            protocol->connectOperation(*itop, timeSliceNode, tautology);
        }
        lastOp.clear();
    }

    std::shared_ptr<MathematicOperable> endTime(new ConstantNumber(finishTime + 2*timeSlice));
    std::shared_ptr<ComparisonOperable> comp1(new SimpleComparison(false, std::dynamic_pointer_cast<MathematicOperable>(time), comparison::less_equal, endTime));

    ProtocolGraph::ProtocolNodePtr loop1 = std::make_shared<LoopNode>(graph_sequence.getNextValue(), comp1);
    protocol->addOperation(loop1);

    protocol->connectOperation(timeSliceNode, loop1, tautology);

    lastOp.push_back(loop1);
    lastComp = comp1;

    return loop1;
}

void BioBlocksJSONReader::parseHeap_createInternalVariables() {
    std::shared_ptr<MathematicOperable> num0 = std::make_shared<ConstantNumber>(0.0);

    int size = operationMap.size();
    for (int i = 0; i < size; i++) {
        std::shared_ptr<VariableEntry> blockVar = std::shared_ptr<VariableEntry>(new VariableEntry("blockVar_" + patch::to_string(i)));
        std::shared_ptr<OperationNode> initOp = std::shared_ptr<OperationNode>(new AssignationOperation(graph_sequence.getNextValue(),blockVar, num0));

        protocol->addOperation(initOp);

        if (!lastOp.empty()) {
            for (auto itop = lastOp.begin(); itop != lastOp.end(); ++itop) {
                protocol->connectOperation(*itop, initOp, tautology);
            }
            lastOp.clear();
        }
        lastOp.push_back(initOp);
    }
}

void BioBlocksJSONReader::parseHeap_processOpBlock (int numBlock,
                                                    long initTime,
                                                    long duration,
                                                    const std::vector<BioBlocks::PipetteOperation> & sameBlockOps,
                                                    std::shared_ptr<OperationNode> & lastIfStart,
                                                    std::shared_ptr<ComparisonOperable> & lastStartComp,
                                                    std::shared_ptr<OperationNode> & lastIfStop,
                                                    std::shared_ptr<ComparisonOperable> & lastStopComp,
                                                    std::shared_ptr<ComparisonOperable> & lastComp)
{
    std::shared_ptr<MathematicOperable> num0 = std::shared_ptr<MathematicOperable>(new ConstantNumber(0.0));
    std::shared_ptr<MathematicOperable> num1 = std::shared_ptr<MathematicOperable>(new ConstantNumber(1.0));

    //create time window if
    std::shared_ptr<ComparisonOperable> yesComp;
    std::shared_ptr<ComparisonOperable> noComp;
    std::shared_ptr<OperationNode> actualIfBlock = createIfBlock(initTime, duration, yesComp, noComp);
    protocol->addOperation(actualIfBlock);

    //connect last op
    for (std::shared_ptr<OperationNode> op: lastOp) {
        protocol->connectOperation(op, actualIfBlock, lastComp);
    }
    lastOp.clear();

    //connect last ifStart no branch
    if (lastIfStart && lastStartComp) {
        protocol->connectOperation(lastIfStart, actualIfBlock, lastStartComp);
    }

    //connect last ifStop no branch
    if (lastIfStop && lastStopComp) {
        protocol->connectOperation(lastIfStop, actualIfBlock, lastStopComp);
    }

    //create actual if flag == 0?
    std::shared_ptr<VariableEntry> blockVar = std::shared_ptr<VariableEntry>(new VariableEntry("blockVar_" + patch::to_string(numBlock)));
    std::shared_ptr<ComparisonOperable> blockFlagUnset = std::shared_ptr<ComparisonOperable>(
                new SimpleComparison(false, std::dynamic_pointer_cast<MathematicOperable>(blockVar),comparison::equal, num0));
    std::shared_ptr<ComparisonOperable> notBlockFlagUnset = std::shared_ptr<ComparisonOperable>(
                new SimpleComparison(true, std::dynamic_pointer_cast<MathematicOperable>(blockVar),comparison::equal, num0));
    std::shared_ptr<DivergeNode> ifStar = std::shared_ptr<DivergeNode>(new DivergeNode(graph_sequence.getNextValue(), blockFlagUnset));
    protocol->addOperation(ifStar);
    protocol->connectOperation(actualIfBlock, ifStar, yesComp);

    //create flag = 1
    std::shared_ptr<OperationNode> setVarFlag = std::shared_ptr<OperationNode>(new AssignationOperation(graph_sequence.getNextValue(), blockVar, num1));
    protocol->addOperation(setVarFlag);
    protocol->connectOperation(ifStar, setVarFlag, blockFlagUnset);

    //create if flag == 1?
    std::shared_ptr<ComparisonOperable> blockFlagSet = std::shared_ptr<ComparisonOperable>(
                new SimpleComparison(false, std::dynamic_pointer_cast<MathematicOperable>(blockVar),comparison::equal, num1));
    std::shared_ptr<ComparisonOperable> notBlockFlagSet = std::shared_ptr<ComparisonOperable>(
                new SimpleComparison(true, std::dynamic_pointer_cast<MathematicOperable>(blockVar),comparison::equal, num1));
    std::shared_ptr<DivergeNode> ifStop = std::shared_ptr<DivergeNode>(new DivergeNode(graph_sequence.getNextValue(), blockFlagSet));
    protocol->addOperation(ifStop);
    protocol->connectOperation(actualIfBlock, ifStop, noComp);

    //create flag = 0
    std::shared_ptr<OperationNode> unsetVarFlag = std::shared_ptr<OperationNode>(new AssignationOperation(graph_sequence.getNextValue(), blockVar, num0));
    protocol->addOperation(unsetVarFlag);
    protocol->connectOperation(ifStop, unsetVarFlag, blockFlagSet);

    //process operations of block
    std::shared_ptr<OperationNode> lastYesBranch = setVarFlag;
    std::shared_ptr<OperationNode> lastNoBranch = unsetVarFlag;

    for (BioBlocks::PipetteOperation pipette: sameBlockOps) {
        std::shared_ptr<OperationNode> pipetteNode = toOperationNode(pipette);
        protocol->addOperation(pipetteNode);
        protocol->connectOperation(lastYesBranch, pipetteNode, tautology);

        pipette.rate = 0;
        std::shared_ptr<OperationNode> pipetteNodeStop = toOperationNode(pipette);
        protocol->addOperation(pipetteNodeStop);
        protocol->connectOperation(lastNoBranch, pipetteNodeStop, tautology);

        lastYesBranch = pipetteNode;
        lastNoBranch = pipetteNodeStop;
    }

    // update next variables
    lastOp.push_back(lastYesBranch);
    lastOp.push_back(lastNoBranch);

    lastIfStart = ifStar;
    lastIfStop = ifStop;
    lastStartComp = notBlockFlagUnset;
    lastStopComp = notBlockFlagSet;
    lastComp = tautology;
}

std::shared_ptr<OperationNode> BioBlocksJSONReader::createIfBlock(long initTime,
                                                                  long duration,
                                                                  shared_ptr<ComparisonOperable> & inCompare,
                                                                  shared_ptr<ComparisonOperable> & outCompare)
{
    shared_ptr<MathematicOperable> init =
            shared_ptr<MathematicOperable>(new ConstantNumber(initTime));
    shared_ptr<MathematicOperable> end =
            shared_ptr<MathematicOperable>(new ConstantNumber(initTime + duration));
    shared_ptr<MathematicOperable> vTime =
            shared_ptr<MathematicOperable>(new VariableEntry(TIME_VARIABLE));

    shared_ptr<ComparisonOperable> compareInit =
            shared_ptr<ComparisonOperable>(new SimpleComparison(false, vTime, comparison::greater_equal, init));
    shared_ptr<ComparisonOperable> compareEnd =
            shared_ptr<ComparisonOperable>(new SimpleComparison(false, vTime, comparison::less, end));
    shared_ptr<ComparisonOperable> andCompare =
            shared_ptr<ComparisonOperable>(new BooleanComparison(false, compareInit, logical::BooleanOperator::conjunction, compareEnd));
    shared_ptr<ComparisonOperable> notAndCompare =
            shared_ptr<ComparisonOperable>(new BooleanComparison(true, compareInit, logical::BooleanOperator::conjunction, compareEnd));

    shared_ptr<OperationNode> ifNode =
            shared_ptr<OperationNode>(new DivergeNode(graph_sequence.getNextValue(),andCompare));

    inCompare = andCompare;
    outCompare = notAndCompare;
    return ifNode;
}

void BioBlocksJSONReader::storeOp(const BioBlocks::PipetteOperation & op) {
    auto it = operationMap.find(std::make_tuple(op.timeOfOperation, op.duration));
    if (it == operationMap.end()) {
        std::vector<BioBlocks::PipetteOperation> ops;
        ops.push_back(op);
        operationMap.insert(std::make_pair(std::make_tuple(op.timeOfOperation, op.duration), ops));
    } else {
        it->second.push_back(op);
    }
}

void BioBlocksJSONReader::parsePipette(const nlohmann::json::iterator & it) {
    string initTime = (*it)["time_of_operation"];
    string duration = (*it)["duration"];

    json op = (*it)["groups"][0];
    json value = op.begin().value();
    string key = op.begin().key();

    json to = value["to"];
    json from = value["from"];

    float initTimeMs = parseTime(initTime);
    float durationMs = parseTime(duration);

    finishTime = max(finishTime, initTimeMs + durationMs);

    if (to.is_array()) { //one to many
        parsePipette_one2many(key, to, from, initTimeMs, durationMs);
    } else if (from.is_array()) { //many to one
        parsePipette_many2one(key, to, from, initTimeMs, durationMs);
    } else { // one to one
        parsePipette_one2one(key, to, from, value, initTimeMs, durationMs);
    }
}

void BioBlocksJSONReader::parsePipette_one2many(const std::string & key,
                                                nlohmann::json  to,
                                                nlohmann::json  from,
                                                float timeInit,
                                                float duration)
{
    for (json::iterator itto = to.begin(); itto != to.end(); ++itto) {
        string targetName = (*itto)["well"].get<string>();
        boost::algorithm::trim(targetName);
        int containerTarget = containerMap.at(targetName);

        if (key.compare("continuous transfer") == 0) {
            string sourceName = from.get<string>();
            boost::algorithm::trim(sourceName);
            int containerSource = containerMap.at(sourceName);

            string rateStr = (*itto)["flow rate"].get<string>();
            float rate = parseFlowRate(rateStr);

            BioBlocks::PipetteOperation operation(timeInit, duration, containerSource, containerTarget, rate, true);
            storeOp(operation);
        } else {
            string sourceName = from.get<string>();
            boost::algorithm::trim(sourceName);
            int containerSource = containerMap.at(sourceName);
            float volume = parseVolume((*itto)["volume"]);

            BioBlocks::PipetteOperation operation(timeInit, duration, containerSource, containerTarget, volume, false);
            storeOp(operation);
        }

    }
}

void BioBlocksJSONReader::parsePipette_many2one(const std::string & key,
                                                json to,
                                                json from,
                                                float timeInit,
                                                float duration)
{
    string destinationName = to.get<string>();
    boost::algorithm::trim(destinationName);
    int containerTarget = containerMap.at(destinationName);

    for (json::iterator itfrom = from.begin(); itfrom != from.end(); ++itfrom) {

        string sourceName = (*itfrom)["well"].get<string>();
        boost::algorithm::trim(sourceName);
        int containerSource = containerMap.at(sourceName);

        if (key.compare("continuous transfer") == 0) {
            string rateStr = (*itfrom)["flow rate"].get<string>();
            float rate = parseFlowRate(rateStr);

            BioBlocks::PipetteOperation operation(timeInit, duration, containerSource, containerTarget, rate, true);
            storeOp(operation);
        }
        else {
            float volume = parseVolume((*itfrom)["volume"]);

            BioBlocks::PipetteOperation operation(timeInit, duration, containerSource, containerTarget, volume, false);
            storeOp(operation);
        }
    }
}

void BioBlocksJSONReader::parsePipette_one2one(const std::string & key,
                                               json to,
                                               json from,
                                               json value,
                                               float timeInit,
                                               float duration)
{
    string destinationName= to.get<string>();
    boost::algorithm::trim(destinationName);
    int containerTarget = containerMap.at(destinationName);

    if (key.compare("continuous transfer") == 0) {
        string sourceName = from.get<string>();
        boost::algorithm::trim(sourceName);
        int containerSource = containerMap.at(sourceName);

        string rateStr = value["flow rate"].get<string>();
        float rate = parseFlowRate(rateStr);

        BioBlocks::PipetteOperation operation(timeInit, duration, containerSource, containerTarget, rate, true);
        storeOp(operation);
    } else {
        string sourceName = from.get<string>();
        boost::algorithm::trim(sourceName);
        int containerSource = containerMap.at(sourceName);

        float volume = parseVolume(value["volume"]);
        BioBlocks::PipetteOperation operation(timeInit, duration, containerSource, containerTarget, volume, false);
        storeOp(operation);
    }
}

float BioBlocksJSONReader::parseVolume(const std::string & text) {
    float value = 0;

    boost::char_separator<char> sep{ ":" };
    boost::tokenizer<boost::char_separator<char>> tok{ text, sep };
    boost::tokenizer< boost::char_separator<char> >::iterator beg = tok.begin();

    string chunk = *beg;
    patch::stod(value, chunk.c_str());

    beg ++;

    string units = *beg;
    return value * toMillilitersPower(units);
}

float BioBlocksJSONReader::parseTime(const std::string & text) {
    float value = 0;

    boost::char_separator<char> sep{ ":" };
    boost::tokenizer<boost::char_separator<char>> tok{ text, sep };
    boost::tokenizer< boost::char_separator<char> >::iterator beg = tok.begin();

    string chunk = *beg;
    patch::stod(value, chunk.c_str());

    beg ++;

    string units = *beg;
    return value * toMillisecondsPower(units);
}

float BioBlocksJSONReader::parseFlowRate(const std::string & text) throw (std::invalid_argument){
    float value = 0;

    boost::char_separator<char> sep{ ":" };
    boost::tokenizer<boost::char_separator<char>> tok{ text, sep };
    boost::tokenizer< boost::char_separator<char> >::iterator beg = tok.begin();

    string chunk = *beg;
    patch::stod(value, chunk.c_str());

    beg ++;

    string units = *beg;
    int posSep = units.find("/");
    if (posSep != string::npos) {
        string volumeUnit = units.substr(0, posSep);
        string timeUnit = units.substr(posSep + 1);

        value = value * (toMillilitersPower(volumeUnit)/toMillisecondsPower(timeUnit));
    } else {
        throw(std::invalid_argument("flow rate units with wrong format, no /"));
    }
    return value;
}

float BioBlocksJSONReader::toMillilitersPower(std::string unit) throw (std::invalid_argument){
    float power;
    if (unit.compare("microliter") == 0) {
        power = 1.0e-3;
    } else if (unit.compare("nanoliter") == 0) {
        power = 1.0e-6;
    } else if (unit.compare("milliliter") == 0) {
        power = 1.0;
    } else {
        throw (std::invalid_argument("unknow volume units: " + unit));
    }
    return power;
}

float BioBlocksJSONReader::toMillisecondsPower(std::string unit) throw (std::invalid_argument){
    float power;
    if (unit.compare("hours") == 0) {
        power = 3.6e+6;
    } else if (unit.compare("minutes") == 0) {
        power = 6.0e+4;
    } else if (unit.compare("seconds") == 0) {
        power = 1.0e+3;
    } else if (unit.compare("milliseconds") == 0) {
        power = 1.0;
    } else {
        throw (std::invalid_argument("unknow time units: " + unit));
    }
    return power;
}

std::shared_ptr<OperationNode> BioBlocksJSONReader::toOperationNode(const BioBlocks::PipetteOperation & op) {
    std::shared_ptr<MathematicOperable> rate = std::make_shared<ConstantNumber>(op.rate);
    std::shared_ptr<OperationNode> rtnNode;
    if (op.continuos) {
        rtnNode = std::shared_ptr<OperationNode>(new SetContinousFlow(graph_sequence.getNextValue(), op.source, op.destination, rate));
    } else {
        rtnNode = std::shared_ptr<OperationNode>(new Transfer(graph_sequence.getNextValue(), op.source, op.destination, rate));
    }
    return rtnNode;
}
