#include "BioBlocksJSONReader.h"

BioBlocksJSONReader* BioBlocksJSONReader::m_pInstance = NULL;

using json = nlohmann::json;

void BioBlocksJSONReader::freeCommandInterface()
{
	if (m_pInstance) {
		delete m_pInstance;
	}
}


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

    try {
        in >> js;

        readRefs(js, protocol);
        readInstructions(js, protocol);
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

void BioBlocksJSONReader::readInstructions(const nlohmann::json & js, ProtocolGraph* protocol) {
    json jrefs = js["instructions"];
    for (json::iterator it = jrefs.begin(); it != jrefs.end(); ++it) {
        string op = (*it)["op"].get<string>();
        if (op.compare("pipette") == 0) {
            parsePipette(it, protocol);
        }
    }
}

void BioBlocksJSONReader::parsePipette(const nlohmann::json::iterator & it, ProtocolGraph* protocol) {
    json op = (*it)["groups"][0];
    json value = op.begin().value();
    string key = op.begin().key();

    json to = value["to"];
    json from = value["from"];

    if (to.is_array()) { //one to many
        parsePipette_one2many(key, to, from, protocol);
    } else if (from.is_array()) { //many to one
        parsePipette_many2one(key, to, from, value, protocol);
    } else { // one to one
        parsePipette_one2one(key, to, from, value, protocol);
    }
}

void BioBlocksJSONReader::parsePipette_one2many(const std::string & key,
                                                nlohmann::json  to,
                                                nlohmann::json  from,
                                                ProtocolGraph* protocol)
{
    std::shared_ptr<OperationNode> actual_op;
    std::vector<std::shared_ptr<OperationNode>> tempList;

    for (json::iterator itto = to.begin(); itto != to.end(); ++itto) {
        string targetName = (*itto)["well"].get<string>();
        boost::algorithm::trim(targetName);
        int containerTarget = containerMap.at(targetName);

        if (key.compare("continuous transfer") == 0) {
            string sourceName = from["well"].get<string>();
            boost::algorithm::trim(sourceName);
            int containerSource = containerMap.at(sourceName);

            string volumeStr = from["flow rate"].get<string>();
            float volume;
            patch::stod(volume, volumeStr.c_str());
            std::shared_ptr<MathematicOperable> volumeObject = std::make_shared<ConstantNumber>(volume);

            actual_op = std::make_shared<SetContinousFlow>(graph_sequence.getNextValue(), containerSource, containerTarget, volumeObject);
        }
        else {
            string sourceName = from.get<string>();
            boost::algorithm::trim(sourceName);
            int containerSource = containerMap.at(sourceName);
            float volume = parseVolume((*itto)["volume"]);
            std::shared_ptr<MathematicOperable> volumeObject = std::make_shared<ConstantNumber>(volume);

            actual_op = std::make_shared<Transfer>(graph_sequence.getNextValue(), containerSource, containerTarget, volumeObject);
        }

        protocol->addOperation(actual_op);
        if (!lastOp.empty()) {
            for (auto itop = lastOp.begin(); itop != lastOp.end(); ++itop) {
                protocol->connectOperation(*itop, actual_op, tautology);
            }
        }
        tempList.push_back(actual_op);
    }

    lastOp.clear();
    for (auto itop = tempList.begin(); itop != tempList.end(); ++itop) {
        lastOp.push_back(*itop);
    }
}

void BioBlocksJSONReader::parsePipette_many2one(const std::string & key,
                                                json to,
                                                json from,
                                                json value,
                                                ProtocolGraph* protocol)
{
    std::shared_ptr<OperationNode> actual_op;
    string destinationName = to.get<string>();
    boost::algorithm::trim(destinationName);
    int containerTarget = containerMap.at(destinationName);

    std::vector<std::shared_ptr<OperationNode>> tempList;
    for (json::iterator itfrom = from.begin(); itfrom != from.end(); ++itfrom) {

        string sourceName = (*itfrom)["well"].get<string>();
        boost::algorithm::trim(sourceName);
        int containerSource = containerMap.at(sourceName);

        if (key.compare("continuous transfer") == 0) {
            string volumeStr = value["flow rate"].get<string>();
            float volume;
            patch::stod(volume, volumeStr.c_str());

            std::shared_ptr<MathematicOperable> volumeObject = std::make_shared<ConstantNumber>(volume);
            actual_op = std::make_shared<SetContinousFlow>(graph_sequence.getNextValue(), containerSource, containerTarget, volumeObject);
        }
        else {
            float volume = parseVolume((*itfrom)["volume"]);
            std::shared_ptr<MathematicOperable> volumeObject = std::make_shared<ConstantNumber>(volume);
            actual_op = std::make_shared<Transfer>(graph_sequence.getNextValue(), containerSource, containerTarget, volumeObject);
        }

        protocol->addOperation(actual_op);
        if (!lastOp.empty()) {
            for (auto itop = lastOp.begin(); itop != lastOp.end(); ++itop) {
                protocol->connectOperation(*itop, actual_op, tautology);
            }
        }
        tempList.push_back(actual_op);
    }
    lastOp.clear();
    for (auto itop = tempList.begin(); itop != tempList.end(); ++itop) {
        lastOp.push_back(*itop);
    }
}

void BioBlocksJSONReader::parsePipette_one2one(const std::string & key,
                                               json to,
                                               json from,
                                               json value,
                                               ProtocolGraph* protocol)
{
    std::shared_ptr<OperationNode> actual_op;
    string destinationName= to.get<string>();
    boost::algorithm::trim(destinationName);
    int containerTarget = containerMap.at(destinationName);

    if (key.compare("continuous transfer") == 0) {
        string sourceName = from["well"].get<string>();
        boost::algorithm::trim(sourceName);
        int containerSource = containerMap.at(sourceName);

        string volumeStr = from["flow rate"].get<string>();
        float volume;
        patch::stod(volume, volumeStr.c_str());

        std::shared_ptr<MathematicOperable> volumeObject = std::make_shared<ConstantNumber>(volume);
        actual_op = std::make_shared<SetContinousFlow>(graph_sequence.getNextValue(), containerSource, containerTarget, volumeObject);
    }
    else {
        string sourceName = from.get<string>();
        boost::algorithm::trim(sourceName);
        int containerSource = containerMap.at(sourceName);

        float volume = parseVolume(value["volume"]);
        std::shared_ptr<MathematicOperable> volumeObject = std::make_shared<ConstantNumber>(volume);
        actual_op = std::make_shared<Transfer>(graph_sequence.getNextValue(), containerSource, containerTarget, volumeObject);
    }

    protocol->addOperation(actual_op);
    if (!lastOp.empty()) {
        for (auto itop = lastOp.begin(); itop != lastOp.end(); ++itop) {
            protocol->connectOperation(*itop, actual_op, tautology);
        }
        lastOp.clear();
    }
    lastOp.push_back(actual_op);
}

float BioBlocksJSONReader::parseVolume(const std::string & text)
{
    float value = 0;

    boost::char_separator<char> sep{ ":" };
    boost::tokenizer<boost::char_separator<char>> tok{ text, sep };
    boost::tokenizer< boost::char_separator<char> >::iterator beg = tok.begin();
    //beg++;

    string chunk = *beg;
    patch::stod(value, chunk.c_str());
    return value;
}
