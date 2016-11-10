/*
 * Mix.cpp
 *
 *  Created on: 23 de mar. de 2016
 *      Author: angel
 */

#include "Mix.h"

Mix::Mix() :
		ContainerOperation() {
	this->idSource1 = -1;
	this->idSource2 = -1;
	this->idTarget = -1;
	this->volume1 = std::shared_ptr<MathematicOperable>();
	this->volume2 = std::shared_ptr<MathematicOperable>();
}

Mix::Mix(const Mix& node) :
		ContainerOperation(node) {
	this->idSource1 = node.idSource1;
	this->idSource2 = node.idSource2;
	this->idTarget = node.idTarget;
	this->volume1 = node.volume1;
	this->volume2 = node.volume2;
}

Mix::Mix(int idConatiner, int idSource1,
		int idSource2, int idTarget,
		std::shared_ptr<MathematicOperable> volume1,
		std::shared_ptr<MathematicOperable> volume2) :
		ContainerOperation(idConatiner) {

	this->idSource1 = idSource1;
	this->idSource2 = idSource2;
	this->idTarget = idTarget;
	this->volume1 = volume1;
	this->volume2 = volume2;
}

Mix::~Mix() {
}

std::string Mix::toText() {
	return patch::to_string(containerID) + "[label=\"Mix("
			+ patch::to_string(idSource1) + ", " + patch::to_string(idSource2)
			+ ", " + patch::to_string(idTarget) + ", "
			+ volume1.get()->toString() + ", " + volume2.get()->toString()
			+ ")\"];";
}

void Mix::loadNode(const std::string& line) throw (invalid_argument) {
	//TODO: JSON
}

void Mix::execute() throw(std::invalid_argument)  {
    getMapping()->mix(idSource1,idSource2,idTarget,volume1.get()->getValue(),volume2.get()->getValue());
}
