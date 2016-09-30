/*
 * LogicalComparison.cpp
 *
 *  Created on: 22 de mar. de 2016
 *      Author: angel
 */

#include "BooleanComparison.h"

BooleanComparison::BooleanComparison(bool negation, std::shared_ptr<ComparisonOperable> left,
		logical::BooleanOperator op,
		std::shared_ptr<ComparisonOperable> right) {

	this->left = left;
	this->right = right;
	this->op = op;
	this->negation = negation;
}

BooleanComparison::~BooleanComparison() {
}

void BooleanComparison::updateReference(const std::string & reference) 
{
	left->updateReference(reference);
	right->updateReference(reference);
}

bool BooleanComparison::conditionMet() {
	bool leftValue = left.get()->conditionMet();
	bool rightValue = right.get()->conditionMet();
	boost::function<bool(bool, bool)> fun = getFunctionType(op);

	bool vuelta = fun(leftValue, rightValue);
	return negation == true ? !vuelta : vuelta;
}

bool BooleanComparison::equal(ComparisonOperable* obj) const {
	bool vuelta = false;
	if (Utils::IsType<BooleanComparison, ComparisonOperable>(obj)) {
		const BooleanComparison* cast = dynamic_cast<const BooleanComparison*>(obj);
		vuelta = ((cast->left.get()->equal(this->left.get()))
				&& (cast->right.get()->equal(this->right.get()))
				&& (cast->op == this->op)
				&& (cast->negation == this->negation));
	}
	return vuelta;
}

boost::function<bool(bool, bool)> BooleanComparison::getFunctionType(
		logical::BooleanOperator op) {

	boost::function<bool(bool, bool)> vuelta;
	switch (op) {
		case logical::conjunction:
			vuelta = std::logical_and<bool>();
			break;
		case logical::disjunction:
			vuelta = std::logical_or<bool>();
			break;
	}
	return vuelta;
}

std::string BooleanComparison::getStringOp() {
	std::string vuelta;
	switch (op) {
	case logical::conjunction:
		vuelta = CONJUNCTION_STRING;
		break;
	case logical::disjunction:
		vuelta = DISJUNCTION_STRING;
		break;
	}
	return vuelta;
}
