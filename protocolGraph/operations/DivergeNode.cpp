/*
 * DivergeNode.cpp
 *
 *  Created on: 11 de abr. de 2016
 *      Author: angel
 */

#include "DivergeNode.h"

DivergeNode::DivergeNode() :
		OperationNode() {
	this->conditionIN = std::shared_ptr<ComparisonOperable>();
}

DivergeNode::DivergeNode(const DivergeNode& node) :
		OperationNode(node) {
	this->conditionIN = node.conditionIN;
}

DivergeNode::DivergeNode(int containerId,
		std::shared_ptr<ComparisonOperable> conditionIN) :
		OperationNode(containerId) {
	this->conditionIN = conditionIN;
}

DivergeNode::~DivergeNode() {}

void DivergeNode::updateReference(const std::string & reference) 
{
	conditionIN->updateReference(reference);
}

string DivergeNode::toText() {
	return patch::to_string(containerID) + "[ label =\"" + DIVERGE_STRING + "("
			+ conditionIN.get()->toString() + ")\"];";
}

void DivergeNode::loadNode(const string& line) throw (invalid_argument) {
	//TODO: JSON
}

void DivergeNode::execute() {
	LOG(DEBUG) << "execute: " << conditionIN.get()->toString();
}
