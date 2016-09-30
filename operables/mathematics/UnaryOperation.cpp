/*
 * UnaryOperation.cpp
 *
 *  Created on: 4 de abr. de 2016
 *      Author: angel
 */

#include "UnaryOperation.h"

UnaryOperation::UnaryOperation() {
	this->variable = shared_ptr<MathematicOperable>();
	this->op = unaryOperations::absoluteValue;
}

UnaryOperation::UnaryOperation(std::shared_ptr<MathematicOperable> variable,
		unaryOperations::UnaryOperator op) {
	this->variable = variable;
	this->op = op;
}

UnaryOperation::~UnaryOperation() 
{

}

void UnaryOperation::updateReference(const std::string & reference) 
{
	variable->updateReference(reference);
}

double UnaryOperation::getValue() throw (std::invalid_argument)  {
	MathematicOperable* value = variable.get();
	double vuelta = 0.0;

	switch(this->op) {
	case unaryOperations::absoluteValue:
		vuelta = fabs(value->getValue());
		break;
	case unaryOperations::floor:
		vuelta = floor(value->getValue());
		break;
	}
	return vuelta;
}

bool UnaryOperation::isPhysical() throw (std::invalid_argument) {
	return (variable.get()->isPhysical());
}

bool UnaryOperation::equal(const MathematicOperable* obj) const {
	bool vuelta = false;
	if (Utils::IsType<UnaryOperation, MathematicOperable>(obj)) {
		const UnaryOperation* cast = dynamic_cast<const UnaryOperation*>(obj);
		vuelta = ((this->variable.get()->equal(cast->variable.get()))
				&& (this->op == cast->op));
	}
	return vuelta;
}

std::string UnaryOperation::getStringOp() {
	std::string vuelta;
	switch (this->op) {
	case unaryOperations::absoluteValue:
		vuelta = ABSOLUTE_VALUE_STRING;
		break;
	case unaryOperations::floor:
		vuelta = FLOOR_VALUE_STRING;
		break;
	}
	return vuelta;
}
