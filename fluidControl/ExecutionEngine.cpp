/*
 * EvoCoder.cpp
 *
 *  Created on: 6 de mar. de 2016
 *      Author: angel
 */

#include "ExecutionEngine.h"

using namespace std;

ExecutionEngine::ExecutionEngine(shared_ptr<ProtocolGraph> protocol,
	std::shared_ptr<VariableTable> table, std::shared_ptr<Mapping> mapping) {
	this->protocol = protocol;
	this->table = table;
	this->mapping = mapping;
	this->actual = NULL;
}

ExecutionEngine::ExecutionEngine(const ExecutionEngine & ec) 
{
	this->protocol = ec.protocol;
	this->table = ec.table;
	this->mapping = ec.mapping;
	this->actual = ec.actual;
}

ExecutionEngine::~ExecutionEngine() {
	//delete mapping;
}

void ExecutionEngine::printProtocol(const std::string & path) {
    protocol->printProtocol(path);
}

bool ExecutionEngine::exec_general() throw (std::runtime_error) {
	bool correct = false;
	CommunicationsInterface::GetInstance()->setTesting(false);
	mapping->settesting(false);
	try {
		sketcher();
        analizeFlows();
		mapping->doMapping();

		table.get()->clear();
		initilizeTime();

		mapping->setExec_general();
		mapping->startCommunications();

		Sleep(3000);
		
		correct = exec();
        mapping->stopCommunications();
        mapping->cleanUsedResources();
	}
	catch (std::invalid_argument& e) {
        mapping->cleanUsedResources();
		throw(std::runtime_error("exception while executing, " + std::string(e.what())));
	}
	catch (std::ios_base::failure& e) {
        mapping->cleanUsedResources();
		throw(std::runtime_error("exception while connecting, " + std::string(e.what())));
	}
	catch (std::runtime_error & e) {
        mapping->cleanUsedResources();
		throw(std::runtime_error("exception while executing, " + std::string(e.what())));
    }
	return		correct;
}

bool ExecutionEngine::exec_ep() throw (std::runtime_error) {
	bool correct = false;
	CommunicationsInterface::GetInstance()->setTesting(false);
	mapping->settesting(false);
	try {
		sketcher();
        analizeFlows();
		mapping->doMapping();

		table.get()->clear();
		initilizeTime();

		mapping->setExec_ep();
		mapping->startCommunications();
		correct = exec();
		mapping->stopCommunications();
	}
	catch (std::invalid_argument& e) {
		throw(std::runtime_error("exception while executing, " + std::string(e.what())));
	}
	catch (std::ios_base::failure& e) {
		throw(std::runtime_error("exception while connecting, " + std::string(e.what())));
	}
	catch (std::runtime_error & e) {
		throw(std::runtime_error("exception while executing, " + std::string(e.what())));
	}
	return correct;
}

bool ExecutionEngine::test() throw (std::runtime_error) {
	bool correct = false;
	CommunicationsInterface::GetInstance()->setTesting(true);
	try {
		sketcher();
        analizeFlows();
        LOG(DEBUG) << "printing minimal machine...";
        mapping->printSketch("sketch_protocol.graph");

		mapping->doMapping();

        mapping->logMappingTable();

		table.get()->clear();
		initilizeTime();

		mapping->setExec_general();
		mapping->settesting(true);
		mapping->startCommunications();
		correct = exec();
		mapping->stopCommunications();
        mapping->cleanUsedResources();

    } catch (std::invalid_argument& e) {
        mapping->cleanUsedResources();
		throw(std::runtime_error("exception while executing, " + std::string(e.what())));
	}
	catch (std::ios_base::failure& e) {
        mapping->cleanUsedResources();
		throw(std::runtime_error("exception while connecting, " + std::string(e.what())));
	}
	catch (std::runtime_error & e) {
        mapping->cleanUsedResources();
		throw(std::runtime_error("exception while executing, " + std::string(e.what())));
	}
	return correct;
}

bool ExecutionEngine::sketcher() {
	table.get()->clear();
	initilizeTime();
	mapping->setSketching();
	mapping->settesting(false);

	ProtocolGraph::ProtocolNodeVectorPtr operations = protocol->getAllNodes();
	for (auto it = operations->begin(); it != operations->end(); ++it) {
		ProtocolGraph::ProtocolNodePtr cast = *it;
		try {
			cast->execute();
		}
		catch (std::invalid_argument& e) {
			LOG(WARNING) << e.what();
		}
	}
	return true;
}

bool ExecutionEngine::exec() {
	bool vuelta = true;
	actual = protocol->getStart();

	if (actual != NULL) {
		ProtocolNodeQueue pending;
		pending.push(actual);

		while (!pending.empty()) {
			actual = pending.front();
			pending.pop();
			actual->execute();
			addAvailableEdges(pending);
		}
	}
	else {
		LOG(FATAL) << "protocol does not has an starting node";
		vuelta = false;
	}
	return vuelta;
}

void ExecutionEngine::addAvailableEdges(ProtocolNodeQueue & nodes) {
	const ProtocolGraph::ProtocolEdgeVectorPtr neighbor = protocol->getProjectingEdges(
		actual->getContainerId());

	LOG(DEBUG) << "adding " << neighbor->size() << " neighbors...";
	for (auto it = neighbor->begin(); it != neighbor->end(); ++it) {
		ProtocolGraph::ProtocolEdgePtr cast = *it;
		if (cast->conditionMet()) {
			nodes.push(protocol->getNode(cast->getIdTarget()));
		}
	}
}

void ExecutionEngine::initilizeTime() {
	table->setValue(TIME_VARIABLE, 0.0);
	mapping->setTimestamp(Utils::getCurrentTimeMilis());
}

bool ExecutionEngine::analizeFlows() {
    LOG(DEBUG) << "analizing active flows";
    bool correct;

    mapping->setAnalizeFlowInTime();
    mapping->clearFlowGenerator();
    table.get()->clear();
    initilizeTime();

    correct = exec();
    return correct;
}
