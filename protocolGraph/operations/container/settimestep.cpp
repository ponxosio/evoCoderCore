#include "settimestep.h"

// Node methods
SetTimeStep::SetTimeStep() :
    ContainerOperation()
{

}

SetTimeStep::SetTimeStep(const SetTimeStep & node) :
    ContainerOperation(node)
{
    this->timeSlice = node.timeSlice;
}

SetTimeStep::SetTimeStep(int containerId, std::shared_ptr<MathematicOperable> timeSlice) :
    ContainerOperation(containerId)
{
    this->timeSlice = timeSlice;
}

SetTimeStep::~SetTimeStep() {

}

std::string SetTimeStep::toText() {
    return patch::to_string(containerID) + "[label=\"SetTimeStep(" + (timeSlice ? patch::to_string(timeSlice->getValue()) : "null") + ")\"]";
}

void SetTimeStep::loadNode(const std::string & line) throw (invalid_argument) {

}

void SetTimeStep::execute() throw(std::invalid_argument) {
    getMapping()->setTimeStep(timeSlice->getValue());
}

void SetTimeStep::updateReference(const std::string & reference) {
    ContainerOperation::updateReference(reference);
}
