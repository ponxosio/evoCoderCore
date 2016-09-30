/*
 * ProtocolGraph.cpp
 *
 *  Created on: 9 de mar. de 2016
 *      Author: angel
 */

#include "MachineGraph.h"

using namespace std;

//static
void MachineGraph::toJSON(const std::string & path, const MachineGraph & machine)
{
	ofstream o(path);
	LOG(DEBUG) << "serializating MachineGraph " + machine.name + " to " + path;
	cereal::JSONOutputArchive ar(o);
	ar(machine);
}

MachineGraph * MachineGraph::fromJSON(const std::string & path)
{
	ifstream i(path);
	LOG(DEBUG) << "loading Machine from " + path;
	cereal::JSONInputArchive arIn(i);

	MachineGraph machine;
	arIn(machine);
	return new MachineGraph(machine);
}
//


MachineGraph::MachineGraph()
 {
	 this->name = "undefined";
	 this->machine = makeGraph();
 }

MachineGraph::MachineGraph(const MachineGraph & machine)
{
	this->name = machine.name;
	this->machine = machine.machine;
}

 MachineGraph::MachineGraph(string name) {
	this->name = name;
	this->machine = makeGraph();
}

MachineGraph::~MachineGraph() {
	//delete machine;
}

bool MachineGraph::addContainer(int idContainer, std::shared_ptr<ContainerNodeType> type,
		float capacity) {
	bool vuelta = false;
	if (!existsContainer(idContainer)) {
		ContainerNodePtr newNode = createContainerNode(idContainer, type, capacity);
		machine->addNode(newNode);
		vuelta = true;
	}
	return vuelta;
}

bool MachineGraph::connectContainer(int idSource, int idTarget) {
	bool vuelta = false;
	if (existsContainer(idSource) && existsContainer(idTarget)
			&& !areConected(idSource, idTarget)) {
		ContainerEdgePtr newEdge = createContainerEdgePtr(idSource, idTarget);
		machine->addEdge(newEdge);
		vuelta = true;
	}
	return vuelta;
}

bool MachineGraph::areConected(int idSource, int idTarget) {
	return machine->areConnected(idSource, idTarget);
}

bool MachineGraph::hasConections(int idContainer) {
	bool vuelta = false;
	ContainerNodePtr cont = machine->getNode(idContainer);

	if (cont != NULL) {
		const ContainerEdgeVectorPtr neighbors = machine->getLeavingEdges(idContainer);
		vuelta = !neighbors->empty();
	}
	return vuelta;
}

bool MachineGraph::changeContainerType(int idContainer, ContainerType type) {
	bool vuelta = false;

	ContainerNodePtr cont = machine->getNode(idContainer);
	if (cont != NULL) {
		cont->changeContainerType(type);
		vuelta = true;
	}
	return vuelta;
}

bool MachineGraph::changeMovementType(int idContainer, MovementType type) {
	bool vuelta = false;

	ContainerNodePtr cont = machine->getNode(idContainer);
	if (cont != NULL) {
		cont->changeMovementType(type);
		vuelta = true;
	}
	return vuelta;
}

bool MachineGraph::addAddon(int idContainer, AddOnsType type) {
	bool vuelta = false;

	ContainerNodePtr cont = machine->getNode(idContainer);
	if (cont != NULL) {
		cont->addAddon(type);
		vuelta = true;
	}
	return vuelta;
}

bool MachineGraph::removeAddon(int idContainer, AddOnsType type) {
	bool vuelta = false;

	ContainerNodePtr cont = machine->getNode(idContainer);
	if (cont != NULL) {
		cont->removeAddon(type);
		vuelta = true;
	}
	return vuelta;
}

void MachineGraph::printMachine(const std::string& path) {
	machine->saveGraph(path);
}

typename MachineGraph::ContainerNodePtr MachineGraph::createContainerNode(int idContainer, std::shared_ptr<ContainerNodeType> type,
	float capacity) {
	return std::make_shared<ContainerNode>(idContainer, type, capacity);
	
}

typename MachineGraph::ContainerEdgePtr MachineGraph::createContainerEdgePtr(int idSource, int idTarget) {
	return std::make_shared<Edge>(idSource, idTarget);
}

typename MachineGraph::ContainerNodeGraphPtr MachineGraph::makeGraph() {
	return std::make_shared<Graph<ContainerNode, Edge>>();
}