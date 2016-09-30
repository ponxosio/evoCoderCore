/*
 * InletContainer.cpp
 *
 *  Created on: 3 de may. de 2016
 *      Author: angel
 */

#include "../../executable/containers/InletContainer.h"

InletContainer::InletContainer() : ExecutableContainerNode() {
	this->ext = std::shared_ptr<Extractor>();
}

InletContainer::InletContainer(const InletContainer& node) : ExecutableContainerNode(node) {
	this->ext = node.ext;
}

InletContainer::InletContainer(int idContainer, float capacity,
		std::shared_ptr<Extractor> ext) : ExecutableContainerNode(idContainer, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(
				ContainerNodeType::fromIntToMovementType(ext.get()->getMovementType()),
				ContainerType::inlet)), capacity) {
	this->ext = ext;
}

InletContainer::~InletContainer() {}

/*std::string InletContainer::toText() {
	//TODO: por definir
}*/

void InletContainer::loadNode(const std::string& line)
		throw (std::invalid_argument) {
	//TODO:Json
}

void InletContainer::receiveLiquid(double rate) throw (std::runtime_error) {
	throw(std::runtime_error("this container cannot receive liquid"));
}

void InletContainer::extractLiquid(double rate) throw (std::runtime_error) {
	ext.get()->extractLiquid(rate);
}

void InletContainer::setPositionInject(int source, int target) throw (std::runtime_error) {
}

void InletContainer::setPositionExtract(int source, int target) throw (std::runtime_error) {
}

void InletContainer::connectContainer(int source, int target) throw (std::runtime_error) {
}

void InletContainer::updateCommunicationInterface(int communication) {
	this->ext->setCommunications(communication);
}