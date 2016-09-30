/*
 * BidirectionalSwitch.cpp
 *
 *  Created on: 3 de may. de 2016
 *      Author: angel
 */

#include "../../executable/containers/BidirectionalSwitch.h"

BidirectionalSwitch::BidirectionalSwitch() : ExecutableContainerNode() {
	this->extract = std::shared_ptr<Extractor>();
	this->insert = std::shared_ptr<Injector>();
	this->controlIN = std::shared_ptr<Control>();
	this->controlOUT = std::shared_ptr<Control>();
}

BidirectionalSwitch::BidirectionalSwitch(const BidirectionalSwitch& node) : ExecutableContainerNode(node) {
	this->extract =  node.extract;
	this->insert =  node.insert;
	this->controlIN = node.controlIN;
	this->controlOUT = node.controlOUT;
}

BidirectionalSwitch::BidirectionalSwitch(int idContainer, float capacity,
		std::shared_ptr<Extractor> extract,
		std::shared_ptr<Injector> insert,
		std::shared_ptr<Control> controlIN,
		std::shared_ptr<Control> controlOUT) :
		ExecutableContainerNode(idContainer, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(
				ContainerNodeType::fromIntToMovementType(extract.get()->getMovementType()),
				ContainerType::bidirectional_switch)), capacity) {

	this->extract = extract;
	this->insert = insert;
	this->controlIN = controlIN;
	this->controlOUT = controlOUT;
}

BidirectionalSwitch::~BidirectionalSwitch() {

}

/*std::string BidirectionalSwitch::toText() {
	//TODO: DEFINIR
}*/

void BidirectionalSwitch::loadNode(const std::string& line)
		throw (std::invalid_argument) {
	//TODO: JSON
}

void BidirectionalSwitch::receiveLiquid(double rate)
		throw (std::runtime_error) {
	insert.get()->injectLiquid(rate);
}

void BidirectionalSwitch::extractLiquid(double rate)
		throw (std::runtime_error) {
	extract.get()->extractLiquid(rate);
}

void BidirectionalSwitch::setPositionExtract(int source, int target) throw (std::runtime_error) {
	controlOUT.get()->setConnection(source, target);
}

void BidirectionalSwitch::setPositionInject(int source, int target) throw (std::runtime_error) {
	controlIN.get()->setConnection(source, target);
}

void BidirectionalSwitch::connectContainer(int source, int target) throw (std::runtime_error) {
	if (containerID == source) {
		controlOUT.get()->addConnection(source, target);
	} else if (containerID == target) {
		controlIN.get()->addConnection(source, target);
	}
}

void BidirectionalSwitch::updateCommunicationInterface(int communication) {
	this->controlIN->setCommunications(communication);
	this->controlOUT->setCommunications(communication);
	this->extract->setCommunications(communication);
	this->insert->setCommunications(communication);
}

void BidirectionalSwitch::clearConnectedContainers() throw (std::runtime_error)
{
	this->controlIN->clearConnections();
	this->controlOUT->clearConnections();
}
