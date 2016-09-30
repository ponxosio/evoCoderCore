/*
 * AsignationOperation.cpp
 *
 *  Created on: 23 de mar. de 2016
 *      Author: angel
 */

#include "AssignationOperation.h"

AssignationOperation::AssignationOperation() : OperationNode() {
	receiver = std::shared_ptr<VariableEntry>();
	value = std::shared_ptr<MathematicOperable>();
}

AssignationOperation::AssignationOperation(const AssignationOperation& obj) :
		OperationNode(obj) {
	this->receiver = obj.receiver;
	this->value = obj.value;
}

AssignationOperation::AssignationOperation(int idContainer,
		std::shared_ptr<VariableEntry> receiver,
		std::shared_ptr<MathematicOperable> value) :
		OperationNode(idContainer) {
	this->receiver = receiver;
	this->value = value;
}

AssignationOperation::~AssignationOperation() {
}

void AssignationOperation::updateReference(const std::string & reference) 
{
	receiver->updateReference(reference);
	value->updateReference(reference);
}

std::string AssignationOperation::toText() {
	return patch::to_string(containerID) + "[label=\""
			+ receiver.get()->toString() + " = " + value.get()->toString()
			+ "\"];";
}

void AssignationOperation::loadNode(const string& line) throw (invalid_argument) {
	//TODO: implementar ya en json
}

void AssignationOperation::execute() throw(std::invalid_argument)  {
	LOG(DEBUG) << "executing: " << receiver.get()->toString() << " = " << value.get()->toString();
	receiver.get()->setValue(value.get()->getValue());
	receiver.get()->setPhysical(value.get()->isPhysical());
}
