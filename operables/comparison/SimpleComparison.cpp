/*
 * SimpleComparison.cpp
 *
 *  Created on: 22 de mar. de 2016
 *      Author: angel
 */

#include "SimpleComparison.h"

SimpleComparison::SimpleComparison(bool negation,std::shared_ptr<MathematicOperable> left,
		comparison::ComparisonOperator op,
		std::shared_ptr<MathematicOperable> right) {

	this->left = left;
	this->right = right;
	this->op = op;
	this->negation = negation;
}

SimpleComparison::~SimpleComparison() {
	// TODO Auto-generated destructor stub
}

void SimpleComparison::updateReference(const std::string & reference) 
{
	left->updateReference(reference);
	right->updateReference(reference);
}

bool SimpleComparison::conditionMet() {
	double leftValue = left.get()->getValue();
	double rightValue = right.get()->getValue();
	boost::function<bool(double, double)> fun = getFunctionType(op);

	bool vuelta = fun(leftValue, rightValue);
	return negation ? !vuelta : vuelta;
}

bool SimpleComparison::equal(ComparisonOperable* obj) const {
	bool vuelta = false;
	if (Utils::IsType<SimpleComparison, ComparisonOperable>(obj)) {
		const SimpleComparison* cast = dynamic_cast<const SimpleComparison*>(obj);
		vuelta = ((cast->left.get()->equal(this->left.get()))
				&& (cast->right.get()->equal(this->right.get()))
				&& (cast->op == this->op)
				&& (cast->negation == this->negation));
	}
	return vuelta;
}

boost::function<bool(double, double)> SimpleComparison::getFunctionType(
		comparison::ComparisonOperator op) {

	boost::function<bool(double, double)> vuelta;
	switch (op) {
	case comparison::less_equal:
		vuelta = std::less_equal<double>();
		break;
	case comparison::less:
		vuelta = std::less<double>();
		break;
	case comparison::greater_equal:
		vuelta = std::greater_equal<double>();
		break;
	case comparison::greater:
		vuelta = std::greater<double>();
		break;
	case comparison::equal:
		vuelta = std::equal_to<double>();
	}
	return vuelta;
}

std::string SimpleComparison::getStingOp() {
	std::string vuelta;
	switch (op) {
	case comparison::less_equal:
		vuelta = LESS_EQUAL_STRING;
		break;
	case comparison::less:
		vuelta = LESS_STRING;
		break;
	case comparison::greater_equal:
		vuelta = GREAT_EQUAL_STRING;
		break;
	case comparison::greater:
		vuelta = GREAT_STRING;
		break;
	case comparison::equal:
		vuelta = EQUAL_STRING;
	}
	return vuelta;
}
