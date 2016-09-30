/*
 * SinkContainer.cpp
 *
 *  Created on: 3 de may. de 2016
 *      Author: angel
 */

#include "../../executable/containers/SinkContainer.h"

SinkContainer::SinkContainer() :
		ExecutableContainerNode() {
	this->insert = std::shared_ptr<Injector>();
}

SinkContainer::SinkContainer(const SinkContainer& node) :
		ExecutableContainerNode(node) {
	this->insert = node.insert;
}

SinkContainer::SinkContainer(int idContainer, float capacity,
		std::shared_ptr<Injector> insert) :
		ExecutableContainerNode(idContainer, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(
				MovementType::irrelevant,
				ContainerType::sink)), capacity) {
	this->insert = insert;
}

SinkContainer::~SinkContainer() {
}

/*std::string SinkContainer::toText() {
	//TODO: definir
}*/

void SinkContainer::loadNode(const std::string& line)
		throw (std::invalid_argument) {
	//TODO: JSON
}

void SinkContainer::receiveLiquid(double rate)
		throw (std::runtime_error) {
	insert.get()->injectLiquid(rate);
}

void SinkContainer::extractLiquid(double rate)
		throw (std::runtime_error) {
	throw(std::invalid_argument("liquid cannot be extracted from this container "));
}

void SinkContainer::setPositionInject(int source, int target) throw (std::runtime_error) {
}

void SinkContainer::setPositionExtract(int source, int target) throw (std::runtime_error) {
}

void SinkContainer::connectContainer(int source, int target) throw (std::runtime_error) {
}

void SinkContainer::updateCommunicationInterface(int communication) {
	this->insert->setCommunications(communication);
}
