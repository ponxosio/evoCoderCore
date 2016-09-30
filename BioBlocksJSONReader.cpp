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


BioBlocksJSONReader::~BioBlocksJSONReader()
{
}

ProtocolGraph* BioBlocksJSONReader::loadFile(const std::string & path)
{
	ProtocolGraph* protocol = new ProtocolGraph("bioBlocks_graph");
	std::shared_ptr<ComparisonOperable> tautology = std::make_shared<Tautology>();
	std::ifstream in(path);
	json js;

	try {
		std::vector<std::shared_ptr<OperationNode>> lastOp;

		in >> js;
		
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
				else {
					protocol->setStartNode(0);
				}

				lastOp.push_back(actual_op);
			} else {
                LOG(ERROR) << "two containers has the same name: " << (*it)["id"];
			}
		}

		jrefs = js["instructions"];

		for (json::iterator it = jrefs.begin(); it != jrefs.end(); ++it) {
			json op = (*it)["groups"][0];
			json value = op.begin().value();
			string key = op.begin().key();

			json to = value["to"];
			json from = value["from"];

			std::shared_ptr<OperationNode> actual_op;
			if (to.is_array()) { //one to many
				
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
					else {
						protocol->setStartNode(0);
					}
					tempList.push_back(actual_op);
				}

				lastOp.clear();
				for (auto itop = tempList.begin(); itop != tempList.end(); ++itop) {
					lastOp.push_back(*itop);
				}
			}
			else if (from.is_array()) { //many to one
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
					else {
						protocol->setStartNode(0);
					}
					tempList.push_back(actual_op);
				}
				lastOp.clear();
				for (auto itop = tempList.begin(); itop != tempList.end(); ++itop) {
					lastOp.push_back(*itop);
				}
			}
			else { // one to one
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
				else {
					protocol->setStartNode(0);
				}

				lastOp.push_back(actual_op);
			}
		}

	}
	catch (std::exception & e) {
		LOG(ERROR) << e.what();
	}

	return protocol;
	//return NULL;
}
