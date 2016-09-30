/*
 * ProtocolGraph.cpp
 *
 *  Created on: 9 de mar. de 2016
 *      Author: angel
 */

#include "ProtocolGraph.h"

//static

void ProtocolGraph::toJSON(const std::string & path, const ProtocolGraph & protocol) {
	ofstream o(path);
	LOG(DEBUG) << "serializating ProtocolGraph " + protocol.name + " to " + path;
	cereal::JSONOutputArchive ar(o);
	ar(CEREAL_NVP(protocol));
}
ProtocolGraph* ProtocolGraph::fromJSON(const std::string & path) {
	ifstream i(path);
	LOG(DEBUG) << "loading Protocol from " + path;
	cereal::JSONInputArchive arIn(i);

	ProtocolGraph protocol;
	arIn(CEREAL_NVP(protocol));
	return new ProtocolGraph(protocol);
}
//

ProtocolGraph::ProtocolGraph() {
	this->name = "undefined";
	this->idStart = -1;
	this->graph = std::make_shared<Graph<OperationNode, ConditionEdge>>();
}

ProtocolGraph::ProtocolGraph(const ProtocolGraph & prot) {
	this->name = prot.name;
	this->idStart = prot.idStart;
	this->graph = prot.graph;
}

ProtocolGraph::ProtocolGraph(const std::string & name) {
	this->name = name;
	this->idStart = -1;
	this->graph = std::make_shared<Graph<OperationNode, ConditionEdge>>();
}

ProtocolGraph::~ProtocolGraph() {}

bool ProtocolGraph::addOperation(ProtocolNodePtr node) {
	return graph->addNode(node);
}

bool ProtocolGraph::connectOperation(ProtocolEdgePtr edge) {
	return graph->addEdge(edge);
}

void ProtocolGraph::updateReference(const std::string & reference) {
	ProtocolNodeVectorPtr nodes = graph->getAllNodes();
	for (auto it = nodes->begin(); it != nodes->end(); ++it) {
		(*it)->updateReference(reference);
	}

	ProtocolEdgeVectorPtr edges = graph->getEdgeList();
	for (auto it = edges->begin(); it != edges->end(); ++it) {
		(*it)->updateReference(reference);
	}
}


ProtocolGraph::ProtocolNodePtr ProtocolGraph::getStart() {
	return graph->getNode(idStart);
}

void ProtocolGraph::setStartNode(int idStart) {
	this->idStart = idStart;
}

bool ProtocolGraph::connectOperation(ProtocolNodePtr nodeSource,
		ProtocolNodePtr nodeTarget,
		std::shared_ptr<ComparisonOperable> comparison) {

	ProtocolEdgePtr edge = makeEdge(nodeSource->getContainerId(), nodeTarget->getContainerId(), comparison);
	return connectOperation(edge);
}

ProtocolGraph::ProtocolEdgePtr ProtocolGraph::makeEdge(int idSource, int idTarget, std::shared_ptr<ComparisonOperable> comparison) {
	//return new ConditionEdge(idSource, idTarget, comparison);
	return std::make_shared<ConditionEdge>(idSource, idTarget, comparison);
}
