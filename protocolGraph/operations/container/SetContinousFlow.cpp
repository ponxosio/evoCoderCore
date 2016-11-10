/*
 * SetContinousFlow.cpp
 *
 *  Created on: 23 de mar. de 2016
 *      Author: angel
 */

#include "SetContinousFlow.h"

SetContinousFlow::SetContinousFlow() :
		ContainerOperation() {
	this->idSource = -1;
	this->idTarget = -1;
	this->rate = std::shared_ptr<MathematicOperable>();
}

SetContinousFlow::SetContinousFlow(const SetContinousFlow& obj) :
		ContainerOperation(obj) {
	this->idSource = obj.idSource;
	this->idTarget = obj.idTarget;
	this->rate = obj.rate;
}

SetContinousFlow::SetContinousFlow(int idContainer,
		int idSource, int idTarget,
		std::shared_ptr<MathematicOperable> rate) :
		ContainerOperation(idContainer) {

	this->idSource = idSource;
	this->idTarget = idTarget;
	this->rate = rate;
}

SetContinousFlow::~SetContinousFlow() {
}

std::string SetContinousFlow::toText() {
	return patch::to_string(containerID) + "[label=\"setContinousFlow("
			+ patch::to_string(idSource) + ", " + patch::to_string(idTarget)
			+ ", " + rate.get()->toString() + ")\"];";
}

void SetContinousFlow::loadNode(const std::string& line)
		throw (invalid_argument) {
	//TODO: cuando se decida la sintaxis JSON
}

void SetContinousFlow::execute() throw(std::invalid_argument)  {
    getMapping()->setContinuosFlow(idSource, idTarget, rate.get()->getValue());
}
