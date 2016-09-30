/*
 * AritmeticOperation.cpp
 *
 *  Created on: 21 de mar. de 2016
 *      Author: angel
 */

#include "ArithmeticOperation.h"

ArithmeticOperation::ArithmeticOperation() {
	this->leftVariable = shared_ptr<MathematicOperable>();
	this->rightVariable = shared_ptr<MathematicOperable>();
	this->op = arithmetic::plus;
}

ArithmeticOperation::ArithmeticOperation(
		std::shared_ptr<MathematicOperable> left,
		arithmetic::ArithmeticOperator op,
		std::shared_ptr<MathematicOperable> right) {

	this->leftVariable = left;
	this->rightVariable = right;
	this->op = op;
}

ArithmeticOperation::~ArithmeticOperation() {

}

void ArithmeticOperation::updateReference(const std::string & reference) {
	leftVariable->updateReference(reference);
	rightVariable->updateReference(reference);
}

double ArithmeticOperation::getValue() throw (std::invalid_argument)  {
	MathematicOperable* left = leftVariable.get();
	MathematicOperable* right = rightVariable.get();
	boost::function<double(double, double)> op = getFunctionType(this->op);

	return (op(left->getValue(), right->getValue()));
}

bool ArithmeticOperation::equal(const MathematicOperable* obj) const {
	bool vuelta = false;
	if (Utils::IsType<ArithmeticOperation, MathematicOperable>(obj)) {
		const ArithmeticOperation* cast = dynamic_cast<const ArithmeticOperation*>(obj);
		vuelta = ((this->leftVariable.get()->equal(cast->leftVariable.get()))
				&& (this->rightVariable.get()->equal(cast->rightVariable.get()))
				&& (this->op == cast->op));
	}
	return vuelta;
}

boost::function<double(double, double)> ArithmeticOperation::getFunctionType(
		arithmetic::ArithmeticOperator op) {

	boost::function<double(double, double)> vuelta;
	switch (op) {
	case arithmetic::plus:
		vuelta = std::plus<double>();
		break;
	case arithmetic::minus:
		vuelta = std::minus<double>();
		break;
	case arithmetic::multiply:
		vuelta = std::multiplies<double>();
		break;
	case arithmetic::divide:
		vuelta = std::divides<double>();
		break;
	}
	return vuelta;
}

std::string ArithmeticOperation::getStringOp() {
	std::string vuelta;
		switch (op) {
		case arithmetic::plus:
			vuelta = PLUS_STRING;
			break;
		case arithmetic::minus:
			vuelta = MINUS_STRING;
			break;
		case arithmetic::multiply:
			vuelta = MULTIPLY_STRING;
			break;
		case arithmetic::divide:
			vuelta = DIVIDE_STRING;
			break;
		}
		return vuelta;
}
