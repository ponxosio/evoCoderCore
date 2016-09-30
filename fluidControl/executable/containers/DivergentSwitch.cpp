/*
 * DivergentSwitch.cpp
 *
 *  Created on: 3 de may. de 2016
 *      Author: angel
 */

#include "../../executable/containers/DivergentSwitch.h"

DivergentSwitch::DivergentSwitch() :
		ExecutableContainerNode() {
	this->extract = std::shared_ptr<Extractor>();
	this->control = std::shared_ptr<Control>();
}

DivergentSwitch::DivergentSwitch(const DivergentSwitch& node) :
		ExecutableContainerNode(node) {
	this->extract = node.extract;
	this->control = node.control;
}

DivergentSwitch::DivergentSwitch(int idConatiner, float capacity,
		std::shared_ptr<Extractor> extract,
		std::shared_ptr<Control> control) :
		ExecutableContainerNode(idConatiner, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(
				ContainerNodeType::fromIntToMovementType(extract.get()->getMovementType()),
				ContainerType::divergent_switch)),
				capacity) {
	this->extract = extract;
	this->control = control;
}

DivergentSwitch::~DivergentSwitch() {

}

/*std::string DivergentSwitch::toText() {
	//TODO: definir
}*/

void DivergentSwitch::loadNode(const std::string& line)
		throw (std::invalid_argument) {
	//TODO: JSON
}

void DivergentSwitch::receiveLiquid(double rate)
		throw (std::runtime_error) {
	throw(std::runtime_error("this container cannot receive liquid"));
}

void DivergentSwitch::extractLiquid(double rate)
		throw (std::runtime_error) {

	extract.get()->extractLiquid(rate);
}

void DivergentSwitch::setPositionInject(int source, int target) throw (std::runtime_error) {
}

void DivergentSwitch::setPositionExtract(int source, int target) throw (std::runtime_error) {
	control.get()->setConnection(source, target);
}

void DivergentSwitch::connectContainer(int source, int target) throw (std::runtime_error) {
	if (containerID == source) {
		control.get()->addConnection(source, target);
	}
}

void DivergentSwitch::updateCommunicationInterface(int communication) {
	this->control->setCommunications(communication);
	this->extract->setCommunications(communication);
}

void DivergentSwitch::clearConnectedContainers() throw (std::runtime_error)
{
	this->control->clearConnections();
}
