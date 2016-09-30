/*
 * ContainerNode.cpp
 *
 *  Created on: 8 de mar. de 2016
 *      Author: angel
 */

#include "ContainerNode.h"

using namespace std;

ContainerNode::ContainerNode(int idConatiner, std::shared_ptr<ContainerNodeType> type, float capacity) :
		Node(idConatiner) {
	this->type = type;
	this->capacity = capacity;
	this->volume = 0.0f;
}

ContainerNode::ContainerNode() :
		Node() {
	this->type = std::shared_ptr<ContainerNodeType>(new ContainerNodeType(MovementType::irrelevant, ContainerType::unknow));
	this->volume = 0.0f;
	this->capacity = 0.0f;
}

ContainerNode::ContainerNode(const ContainerNode& node) :
		Node(node) {
	this->type = node.type;
	this->capacity = node.capacity;
	this->volume = volume;
}

ContainerNode::~ContainerNode() {
	// TODO Auto-generated destructor stub
}

string ContainerNode::toText() {
	std::ostringstream stm;
	stm << patch::to_string(this->containerID) << " [image=\""
			<< Utils::getCurrentDir() << this->type.get()->getTypeImagePath()
			<< "\"];";
	stm << this->type.get()->makeAddonsNodes(this->containerID);
	return stm.str();
}

void ContainerNode::changeContainerType(ContainerType type) {
	this->type.get()->changeContainerType(type);
}

void ContainerNode::changeMovementType(MovementType type) {
	this->type.get()->changeMovementType(type);
}

void ContainerNode::addAddon(AddOnsType type) {
	this->type.get()->addAddon(type);
}

void ContainerNode::removeAddon(AddOnsType type) {
	this->type.get()->removeAddon(type);
}
