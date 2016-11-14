#include "BioBlocksJSONReader.h"

// STATIC
BioBlocksJSONReader* BioBlocksJSONReader::m_pInstance = NULL;
long BioBlocksJSONReader::timeSlice = 1000;

void BioBlocksJSONReader::freeCommandInterface()
{
    if (m_pInstance) {
        delete m_pInstance;
    }
}

void BioBlocksJSONReader::setTimeSlice(long timeSlice) {
    BioBlocksJSONReader::timeSlice = timeSlice;
}
//

using json = nlohmann::json;

BioBlocksJSONReader::BioBlocksJSONReader()
{
    tautology = std::make_shared<Tautology>();
}

BioBlocksJSONReader::~BioBlocksJSONReader()
{
}

ProtocolGraph* BioBlocksJSONReader::loadFile(const std::string & path) throw(std::invalid_argument)
{
    ProtocolGraph* protocol = new ProtocolGraph("bioBlocks_graph");
    std::ifstream in(path);
    json js;

    containerMap.clear();
    lastOp.clear();
    table_sequence.reset();
    graph_sequence.reset();
    protocol->setStartNode(0);
    finishTime = 0;
    while(!operationHeap.empty()) {
        operationHeap.pop();
    }

    try {
        in >> js;
        readRefs(js, protocol);

        readInstructions(js, operationHeap);
        parseHeap(operationHeap, protocol);

        LOG(INFO) << "translation done! id for container at the protocol:";
        for (auto it = containerMap.begin(); it != containerMap.end(); ++it) {
            LOG(INFO) << it->first << ", " << it->second;
        }
    } catch (invalid_argument & e) {
        throw(e);
    } catch (exception & e) {
        throw(std::invalid_argument("unknow exception: " + string(e.what())));
    }

    return protocol;
}

void BioBlocksJSONReader::readRefs(const nlohmann::json & js, ProtocolGraph* protocol) throw(std::invalid_argument)
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

void BioBlocksJSONReader::readInstructions(const nlohmann::json & js, OperationsMultiMap & operationHeap) {
    json jrefs = js["instructions"];
    for (json::iterator it = jrefs.begin(); it != jrefs.end(); ++it) {
        string op = (*it)["op"].get<string>();
        if (op.compare("pipette") == 0) {
            parsePipette(it, operationHeap);
        }
    }
}

void BioBlocksJSONReader::parseHeap(OperationsMultiMap & operationHeap, ProtocolGraph* protocol) {
    if (!operationHeap.empty()) {
        std::shared_ptr<MathematicOperable> endTime(new ConstantNumber(finishTime + 2*timeSlice));
        std::shared_ptr<VariableEntry> time(
            new VariableEntry(TIME_VARIABLE));
        std::shared_ptr<MathematicOperable> mtime(
                    new VariableEntry(TIME_VARIABLE));
        std::shared_ptr<ComparisonOperable> comp1(
                    new SimpleComparison(false, mtime, comparison::less_equal, endTime));

        ProtocolGraph::ProtocolNodePtr loop1 = std::make_shared<LoopNode>(graph_sequence.getNextValue(), comp1);

        protocol->addOperation(loop1);

        if (!lastOp.empty()) {
            for (auto itop = lastOp.begin(); itop != lastOp.end(); ++itop) {
                protocol->connectOperation(*itop, loop1, tautology);
            }
            lastOp.clear();
        }
        lastOp.push_back(loop1);

        std::shared_ptr<ComparisonOperable> lastComp = comp1;
        long lastInitTime = operationHeap.top().timeOfOperation;
        long lastDuration = operationHeap.top().duration;

        std::vector<BioBlocks::PipetteOperation> sameBlockOps;
        while (!operationHeap.empty()) {
            BioBlocks::PipetteOperation actual = operationHeap.top();
            if (lastInitTime != actual.timeOfOperation ||
                lastDuration != actual.duration)
            {

            }
            sameBlockOps.push_back(actual);
            operationHeap.pop();
        }

        if (!sameBlockOps.empty()) {

        }

        ProtocolGraph::ProtocolNodePtr timeStep = std::make_shared<TimeStep>(graph_sequence.getNextValue(), time);
        protocol->addOperation(timeStep);

        for (std::shared_ptr<OperationNode> op: lastOp) {
            protocol->connectOperation(op, timeStep, lastComp);
        }
        lastOp.clear();

        protocol->connectOperation(timeStep, loop1, tautology);
    }
}

void BioBlocksJSONReader::parseHeap_createInternalVariables(ProtocolGraph* protocol, OperationsMultiMap & operationHeap) {

}

void BioBlocksJSONReader::parseHeap_processOpBlock (ProtocolGraph* protocol,
                                          const std::vector<BioBlocks::PipetteOperation> & sameBlockOps,
                                          std::shared_ptr<OperationNode> & lastIf,
                                          std::shared_ptr<ComparisonOperable> & lastNoComp,
                                          int & lastInitTime,
                                          int & lastDuration)
{
    std::shared_ptr<ComparisonOperable> yesComp;
    std::shared_ptr<ComparisonOperable> noComp;
    std::shared_ptr<OperationNode> actualIfBlock = createIfBlock(lastInitTime, lastDuration, yesComp, noComp);
    protocol->addOperation(actualIfBlock);

    for (std::shared_ptr<OperationNode> op: lastOp) {
        protocol->connectOperation(op, actualIfBlock, lastComp);
    }
    lastOp.clear();

    std::shared_ptr<OperationNode> lastYesBranch = actualIfBlock;
    std::shared_ptr<OperationNode> lastNoBranch = actualIfBlock;
    std::shared_ptr<ComparisonOperable> lastYesComp = yesComp;
    std::shared_ptr<ComparisonOperable> lastNoComp = noComp;

    for (BioBlocks::PipetteOperation pipette: sameBlockOps) {
        std::shared_ptr<OperationNode> pipetteNode = toOperationNode(pipette);
        protocol->addOperation(pipetteNode);
        protocol->connectOperation(lastYesBranch, pipetteNode, lastYesComp);

        pipette.rate = 0;
        std::shared_ptr<OperationNode> pipetteNodeStop = toOperationNode(pipette);
        protocol->addOperation(pipetteNodeStop);
        protocol->connectOperation(lastNoBranch, pipetteNodeStop, lastNoComp);

        lastYesBranch = pipetteNode;
        lastNoBranch = pipetteNodeStop;
        lastYesComp = tautology;
        lastNoComp = tautology;
    }
    lastOp.push_back(lastYesBranch);
    lastOp.push_back(lastNoBranch);
    sameBlockOps.clear();

    lastComp = tautology;

    lastInitTime = actual.timeOfOperation;
    lastDuration = actual.duration;
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

void BioBlocksJSONReader::parsePipette(const nlohmann::json::iterator & it, OperationsMultiMap & operationHeap) {
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
        parsePipette_one2many(key, to, from, initTimeMs, durationMs, operationHeap);
    } else if (from.is_array()) { //many to one
        parsePipette_many2one(key, to, from, value, initTimeMs, durationMs, operationHeap);
    } else { // one to one
        parsePipette_one2one(key, to, from, value, initTimeMs, durationMs, operationHeap);
    }
}

void BioBlocksJSONReader::parsePipette_one2many(const std::string & key,
                                                nlohmann::json  to,
                                                nlohmann::json  from,
                                                float timeInit,
                                                float duration,
                                                OperationsMultiMap & operationHeap)
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
            operationHeap.insert(std::make_pair(std::make_tuple(operation.timeOfOperation, operation.duration),operation));
        } else {
            string sourceName = from.get<string>();
            boost::algorithm::trim(sourceName);
            int containerSource = containerMap.at(sourceName);
            float volume = parseVolume((*itto)["volume"]);

            BioBlocks::PipetteOperation operation(timeInit, duration, containerSource, containerTarget, volume, false);
            operationHeap.insert(std::make_pair(std::make_tuple(operation.timeOfOperation, operation.duration),operation));
        }

    }
}

void BioBlocksJSONReader::parsePipette_many2one(const std::string & key,
                                                json to,
                                                json from,
                                                json value,
                                                float timeInit,
                                                float duration,
                                                OperationsMultiMap & operationHeap)
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
            operationHeap.insert(std::make_pair(std::make_tuple(operation.timeOfOperation, operation.duration),operation));
        }
        else {
            float volume = parseVolume((*itfrom)["volume"]);

            BioBlocks::PipetteOperation operation(timeInit, duration, containerSource, containerTarget, volume, false);
            operationHeap.insert(std::make_pair(std::make_tuple(operation.timeOfOperation, operation.duration),operation));
        }
    }
}

void BioBlocksJSONReader::parsePipette_one2one(const std::string & key,
                                               json to,
                                               json from,
                                               json value,
                                               float timeInit,
                                               float duration,
                                               OperationsMultiMap & operationHeap)
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
        operationHeap.insert(std::make_pair(std::make_tuple(operation.timeOfOperation, operation.duration),operation));
    } else {
        string sourceName = from.get<string>();
        boost::algorithm::trim(sourceName);
        int containerSource = containerMap.at(sourceName);

        float volume = parseVolume(value["volume"]);
        BioBlocks::PipetteOperation operation(timeInit, duration, containerSource, containerTarget, volume, false);
        operationHeap.insert(std::make_pair(std::make_tuple(operation.timeOfOperation, operation.duration),operation));
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

        value = value * toMillilitersPower(volumeUnit) * (toMillisecondsPower(timeUnit) * 1.0e-1);
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
