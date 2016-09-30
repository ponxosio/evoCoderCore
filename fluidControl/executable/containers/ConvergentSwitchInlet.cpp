/*
 * ConvergentSwitchInlet.cpp
 *
 *  Created on: 4 de may. de 2016
 *      Author: angel
 */

#include "../../executable/containers/ConvergentSwitchInlet.h"

ConvergentSwitchInlet::ConvergentSwitchInlet() :
		ExecutableContainerNode() {
	this->insert = std::shared_ptr<Injector>();
	this->extractor = std::shared_ptr<Extractor>();
	this->control = std::shared_ptr<Control>();
}

ConvergentSwitchInlet::ConvergentSwitchInlet(const ConvergentSwitchInlet& node) :
		ExecutableContainerNode(node) {
	this->insert = node.insert;
	this->extractor = node.extractor;
	this->control = node.control;
}

ConvergentSwitchInlet::ConvergentSwitchInlet(int idConatiner, float capacity,
		std::shared_ptr<Injector> insert,
		std::shared_ptr<Extractor> extractor,
		std::shared_ptr<Control> control) :
		ExecutableContainerNode(idConatiner,
				std::shared_ptr<ContainerNodeType>(new ContainerNodeType(
						ContainerNodeType::fromIntToMovementType(extractor.get()->getMovementType()),
						ContainerType::convergent_switch_inlet)), capacity) {

	this->insert = insert;
	this->extractor = extractor;
	this->control = control;
}

ConvergentSwitchInlet::~ConvergentSwitchInlet() {

}

/*std::string ConvergentSwitchInlet::toText() {
	//TODO: definir
}*/

void ConvergentSwitchInlet::loadNode(const std::string& line)
		throw (std::invalid_argument) {
	//TODO: JSON
}

void ConvergentSwitchInlet::receiveLiquid(double rate)
		throw (std::runtime_error) {
	this->insert.get()->injectLiquid(rate);
}

void ConvergentSwitchInlet::extractLiquid(double rate)
		throw (std::runtime_error) {
	this->extractor.get()->extractLiquid(rate);
}

void ConvergentSwitchInlet::setPositionInject(int source, int target) throw (std::runtime_error) {
	this->control.get()->setConnection(source, target);
}

void ConvergentSwitchInlet::setPositionExtract(int source, int target) throw (std::runtime_error) {
}

void ConvergentSwitchInlet::connectContainer(int source, int target) throw (std::runtime_error) {
	if (containerID == target) {
		this->control.get()->addConnection(source, target);
	}
}

void ConvergentSwitchInlet::updateCommunicationInterface(int communication) {
	this->control->setCommunications(communication);
	this->insert->setCommunications(communication);
	this->extractor->setCommunications(communication);
}

void ConvergentSwitchInlet::clearConnectedContainers() throw (std::runtime_error)
{
	this->control->clearConnections();
}
