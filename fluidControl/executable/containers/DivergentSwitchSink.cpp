/*
 * DivergentSwitchSink.cpp
 *
 *  Created on: 4 de may. de 2016
 *      Author: angel
 */

#include "../../executable/containers/DivergentSwitchSink.h"

DivergentSwitchSink::DivergentSwitchSink() :
		ExecutableContainerNode() {
	this->insert = std::shared_ptr<Injector>();
	this->extractor = std::shared_ptr<Extractor>();
	this->control = std::shared_ptr<Control>();
}

DivergentSwitchSink::DivergentSwitchSink(const DivergentSwitchSink& node) :
		ExecutableContainerNode(node) {
	this->insert = node.insert;
	this->extractor = node.extractor;
	this->control = node.control;
}

DivergentSwitchSink::DivergentSwitchSink(int idConatiner, float capacity,
		std::shared_ptr<Injector> insert,
		std::shared_ptr<Extractor> extractor,
		std::shared_ptr<Control> control) :
		ExecutableContainerNode(idConatiner,
				std::shared_ptr<ContainerNodeType>(new ContainerNodeType(
						ContainerNodeType::fromIntToMovementType(extractor.get()->getMovementType()),
						ContainerType::divergent_switch_sink)), capacity) {

	this->insert = insert;
	this->extractor = extractor;
	this->control = control;
}

DivergentSwitchSink::~DivergentSwitchSink() {

}

/*std::string DivergentSwitchSink::toText() {
	//TODO: definir
}*/

void DivergentSwitchSink::loadNode(const std::string& line)
		throw (std::invalid_argument) {
	//TODO: JSON
}

void DivergentSwitchSink::receiveLiquid(double rate)
		throw (std::runtime_error) {
	this->insert.get()->injectLiquid(rate);
}

void DivergentSwitchSink::extractLiquid(double rate)
		throw (std::runtime_error) {
	this->extractor.get()->extractLiquid(rate);
}

void DivergentSwitchSink::setPositionInject(int source, int target) throw (std::runtime_error) {
}

void DivergentSwitchSink::setPositionExtract(int source, int target) throw (std::runtime_error) {
	this->control.get()->setConnection(source, target);
}

void DivergentSwitchSink::connectContainer(int source, int target) throw (std::runtime_error) {
	if (containerID == source) {
		this->control.get()->addConnection(source, target);
	}
}

void DivergentSwitchSink::updateCommunicationInterface(int communication) {
	this->control->setCommunications(communication);
	this->insert->setCommunications(communication);
	this->extractor->setCommunications(communication);
}

void DivergentSwitchSink::clearConnectedContainers() throw (std::runtime_error)
{
	this->control->clearConnections();
}
