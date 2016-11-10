/*
 * TimeStep.cpp
 *
 *  Created on: 28 de mar. de 2016
 *      Author: angel
 */

#include "TimeStep.h"

TimeStep::TimeStep() :
		ContainerOperation() {
	this->receiver = std::shared_ptr<VariableEntry>();
}

TimeStep::TimeStep(const TimeStep& node) :
		ContainerOperation(node) {
	this->receiver = node.receiver;
}

TimeStep::TimeStep(int containerId, std::shared_ptr<VariableEntry> receiver) :
		ContainerOperation(containerId) {
	this->receiver = receiver;
}

TimeStep::~TimeStep() {
}

std::string TimeStep::toText() {
	return patch::to_string(containerID) + "[label=\"TimeStep()\"];";
}

void TimeStep::loadNode(const std::string& line) throw (invalid_argument) {
	//TODO: JSON
}

void TimeStep::execute() throw(std::invalid_argument)  {
	receiver.get()->setValue(
            receiver.get()->getValue() + getMapping()->timeStept());
    LOG(INFO) << "time variable: " << receiver.get()->getValue();
}

void TimeStep::updateReference(const std::string & reference)
{
    ContainerOperation::updateReference(reference);
    receiver->updateReference(reference);
}
