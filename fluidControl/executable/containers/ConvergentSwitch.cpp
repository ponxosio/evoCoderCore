/*
 * ConvergentSwitch.cpp
 *
 *  Created on: 3 de may. de 2016
 *      Author: angel
 */

#include "../../executable/containers/ConvergentSwitch.h"

ConvergentSwitch::ConvergentSwitch() :
		ExecutableContainerNode() {
	this->insert = std::shared_ptr<Injector>();
	this->control = std::shared_ptr<Control>();
}

ConvergentSwitch::ConvergentSwitch(const ConvergentSwitch& node) {
	this->insert = node.insert;
	this->control = node.control;
}

ConvergentSwitch::ConvergentSwitch(int idConatiner, float capacity,
		std::shared_ptr<Injector> insert, std::shared_ptr<Control> control) :
		ExecutableContainerNode(idConatiner,
				std::shared_ptr<ContainerNodeType>(new ContainerNodeType(
						MovementType::irrelevant,
						ContainerType::convergent_switch)), capacity) {
	this->insert = insert;
	this->control = control;
}

ConvergentSwitch::~ConvergentSwitch() {

}

/*std::string ConvergentSwitch::toText() {
	//TODO definir
}*/

void ConvergentSwitch::loadNode(const std::string& line)
		throw (std::invalid_argument) {
	//TODO: JSON
}

void ConvergentSwitch::receiveLiquid(double rate)
		throw (std::runtime_error) {
	insert.get()->injectLiquid(rate);
}

void ConvergentSwitch::extractLiquid(double rate)
		throw (std::runtime_error) {
	throw(std::runtime_error(
			"liquid cannot be extracted from this container "));
}

void ConvergentSwitch::setPositionInject(int source, int target) throw (std::runtime_error) {
	control.get()->setConnection(source, target);
}

void ConvergentSwitch::setPositionExtract(int source, int target) throw (std::runtime_error) {
}

void ConvergentSwitch::connectContainer(int source, int target) throw (std::runtime_error)  {
	if (containerID == target) {
		control.get()->addConnection(source, target);
	}
}

void ConvergentSwitch::updateCommunicationInterface(int communication) {
	this->control->setCommunications(communication);
	this->insert->setCommunications(communication);
}

void ConvergentSwitch::clearConnectedContainers() throw (std::runtime_error) 
{
	this->control->clearConnections();
}
