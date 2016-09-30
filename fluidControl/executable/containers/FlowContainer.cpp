/*
 * FlowContainer.cpp
 *
 *  Created on: 3 de may. de 2016
 *      Author: angel
 */

#include "../../executable/containers/FlowContainer.h"

FlowContainer::FlowContainer() : ExecutableContainerNode() {
	this->extract = std::shared_ptr<Extractor>();
	this->insert = std::shared_ptr<Injector>();
}

FlowContainer::FlowContainer(const FlowContainer& node) : ExecutableContainerNode(node) {
	this->extract = node.extract;
	this->insert = node.insert;
}

FlowContainer::FlowContainer(int idContainer, float capacity,
		std::shared_ptr<Extractor> extract, std::shared_ptr<Injector> insert) :
		ExecutableContainerNode(idContainer, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(
				ContainerNodeType::fromIntToMovementType(extract.get()->getMovementType()),
				ContainerType::flow)),
				capacity) {

	this->extract = extract;
	this->insert = insert;
}

FlowContainer::~FlowContainer() {
	// TODO Auto-generated destructor stub
}

/*std::string FlowContainer::toText() {
	//TODO: DEFINIR
}*/

void FlowContainer::loadNode(const std::string& line)
		throw (std::invalid_argument) {
	//TODO:JSON
}

void FlowContainer::receiveLiquid(double rate)
		throw (std::runtime_error) {
	this->insert.get()->injectLiquid(rate);
}

void FlowContainer::extractLiquid(double rate)
		throw (std::runtime_error) {
	this->extract.get()->extractLiquid(rate);
}

void FlowContainer::setPositionInject(int source, int target) throw (std::runtime_error) {
}

void FlowContainer::setPositionExtract(int source, int target) throw (std::runtime_error) {
}

void FlowContainer::connectContainer(int source, int target) throw (std::runtime_error) {
}

void FlowContainer::updateCommunicationInterface(int communication) {
	this->insert->setCommunications(communication);
	this->extract->setCommunications(communication);
}
