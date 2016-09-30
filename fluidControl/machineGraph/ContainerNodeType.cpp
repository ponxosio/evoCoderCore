/*
 * ContainerNodeType.cpp
 *
 *  Created on: 4 de may. de 2016
 *      Author: angel
 */

#include "ContainerNodeType.h"

MovementType ContainerNodeType::fromIntToMovementType(int movement) {
	MovementType vuelta;
	switch (movement) {
	case 0:
		vuelta = MovementType::continuous;
		break;
	case 1:
		vuelta = MovementType::discrete;
		break;
	case 2:
		vuelta = MovementType::irrelevant;
		break;
	default:
		break;
	}
	return vuelta;
}

ContainerNodeType::ContainerNodeType() {
	this->movementType = MovementType::irrelevant;
	this->containerType = ContainerType::unknow;

	for (int i = 0; i < AddOnsType::ADDONS_MAX; i++) {
		this->addOns[i] = false;
	}
}

ContainerNodeType::ContainerNodeType(const ContainerNodeType& type) {
	this->movementType = type.movementType;
	this->containerType = type.containerType;

	for (int i = 0; i < AddOnsType::ADDONS_MAX; i++) {
		this->addOns[i] = type.addOns[i];
	}
}

ContainerNodeType::ContainerNodeType(MovementType movementType,
		ContainerType containerType) {
	this->movementType = movementType;
	this->containerType = containerType;

	for (int i = 0; i < AddOnsType::ADDONS_MAX; i++) {
		this->addOns[i] = false;
	}
}

ContainerNodeType::ContainerNodeType(MovementType movementType,
		ContainerType containerType, std::vector<AddOnsType> addOns) {
	this->movementType = movementType;
	this->containerType = containerType;

	for (int i = 0; i < AddOnsType::ADDONS_MAX; i++) {
		this->addOns[i] = false;
	}

	for (auto it = addOns.begin(); it != addOns.end(); ++it) {
		AddOnsType actualAddOn = *it;
		this->addOns[actualAddOn] = true;
	}
}

ContainerNodeType::~ContainerNodeType() {
}

void ContainerNodeType::changeContainerType(ContainerType newContainerType) {
	this->containerType = newContainerType;
}

void ContainerNodeType::changeMovementType(MovementType newMovementType) {
	this->movementType = newMovementType;
}

void ContainerNodeType::addAddon(AddOnsType addOn) {
	this->addOns[addOn] = true;
}

void ContainerNodeType::removeAddon(AddOnsType addOn) {
	this->addOns[addOn] = false;
}

bool ContainerNodeType::isCompatible(const ContainerNodeType& type) {
	return (isCompatibleMovement(type.movementType)
			&& isCompatibleContainer(type.containerType)
			&& isCompatibleAddOns(type.addOns));
}

bool ContainerNodeType::isCompatibleMovement(MovementType movement) {
	return (this->movementType == movement
			|| movement == MovementType::irrelevant
			|| this->movementType == MovementType::irrelevant);
}

bool ContainerNodeType::isCompatibleContainer(ContainerType container) {
	bool compatible = false;

	if (this->containerType != container) {
		switch (container) {
		case inlet:
			compatible = ((this->containerType == bidirectional_switch)
					|| (this->containerType == convergent_switch_inlet)
					|| (this->containerType == divergent_switch_sink)
					|| (this->containerType == divergent_switch) || (this->containerType == flow));
			break;
		case flow:
			compatible = (this->containerType == bidirectional_switch)
					|| (this->containerType == convergent_switch_inlet)
					|| (this->containerType == divergent_switch_sink);
			break;
		case sink:
			compatible = ((this->containerType == bidirectional_switch)
					|| (this->containerType == convergent_switch)
					|| (this->containerType == convergent_switch_inlet)
					|| (this->containerType == divergent_switch_sink)
					|| (this->containerType == flow));
			break;
		case divergent_switch:
			compatible = (this->containerType == bidirectional_switch)
					|| (this->containerType == divergent_switch_sink);
			break;
		case convergent_switch:
			compatible = (this->containerType == bidirectional_switch)
					|| (this->containerType == convergent_switch_inlet);
			break;
		case divergent_switch_sink:
			compatible = (this->containerType == bidirectional_switch);
			break;
		case convergent_switch_inlet:
			compatible = (this->containerType == bidirectional_switch);
			break;
		case bidirectional_switch:
			break;
		case unknow:
			compatible = true;
			break;
		}
	} else {
		compatible = true;
	}
	return compatible;
}

std::string ContainerNodeType::getContainerTypeString() {
	std::string vuelta;
	switch (this->containerType) {
	case ContainerType::inlet:
		vuelta = INLET_NAME;
		break;
	case ContainerType::flow:
		vuelta = FLOW_NAME;
		break;
	case ContainerType::sink:
		vuelta = SINK_NAME;
		break;
	case ContainerType::divergent_switch:
		vuelta = DIVERGENT_SWITCH_NAME;
		break;
	case ContainerType::convergent_switch:
		vuelta = CONVERGENT_SWITCH_NAME;
		break;
	case ContainerType::divergent_switch_sink:
		vuelta = DIVERGENT_SWITCH_SINK_NAME;
		break;
	case ContainerType::convergent_switch_inlet:
		vuelta = CONV_SWITCH_INLET_NAME;
		break;
	case ContainerType::bidirectional_switch:
		vuelta = BIDIRECTIONAL_SWITCH_NAME;
		break;
	case ContainerType::unknow:
		vuelta = UNKNOW_NAME;
		break;
	}
	return vuelta;
}

bool ContainerNodeType::isCompatibleAddOns(const std::array<bool, AddOnsType::ADDONS_MAX> & addOns) {
	bool compatible = true;
	for (int i = 0; compatible && (i < AddOnsType::ADDONS_MAX); i++) {
		if (addOns[i]) { //if the receiving type has an addon, this must have it too
			compatible = this->addOns[i];
		}
	}
	return compatible;
}

void ContainerNodeType::load(const std::string & textType)
		throw (std::invalid_argument) {
	//TODO: JSON
}

bool ContainerNodeType::hasMovementType(MovementType movement) {
	return (this->movementType == movement);
}

bool ContainerNodeType::hasContainerType(ContainerType container) {
	return (this->containerType == container);
}

bool ContainerNodeType::hasAddOns(AddOnsType addOn) {
	return addOns[addOn];
}

std::string ContainerNodeType::getTypeString() {
	std::string name = "movement: " + getMovementTypeString() + ", container: " + getContainerTypeString() + ", addOns: ";
	std::string addons = "";
	for (int i = 0; i < AddOnsType::ADDONS_MAX; i++) {
		addons += patch::to_string(i);
	}
	name += addons;
	return name;
}

std::string ContainerNodeType::makeAddonsNodes(int idContainer) {
	std::string vuelta = "";
	int base = idContainer * (-ADDONS_MAX);
	for (int i = 0; i < ADDONS_MAX; i++) {
		if (addOns[i] == true) {
			vuelta += patch::to_string(base - i) + "[image=\""
					+ Utils::getCurrentDir() + PATH_TO_IMGS
					+ getAddonsImg((AddOnsType) (i)) + "\"];"
					+ patch::to_string(base - i) + "->"
					+ patch::to_string(idContainer) + ";";
		}
	}
	return vuelta;
}

std::string ContainerNodeType::getMovementTypeString() {
	std::string vuelta;
	switch (this->movementType) {
	case MovementType::continuous:
		vuelta = CONTINUOUS_NAME;
		break;
	case MovementType::discrete:
		vuelta = DISCRETE_NAME;
		break;
	case MovementType::irrelevant:
		vuelta = IRRELEVANT_NAME;
		break;
	default:
		vuelta = "";
		break;
	}
	return vuelta;
}

std::string ContainerNodeType::getAddonsImg(AddOnsType addonType) {
	std::string vuelta;
	switch (addonType) {
	case AddOnsType::mixer:
		vuelta = MIXER;
		break;
	case AddOnsType::light:
		vuelta = LIGHT;
		break;
	case AddOnsType::temp:
		vuelta = TEMPERATURE;
		break;
	case AddOnsType::OD_sensor:
		vuelta = OD_SENSOR;
		break;
	default:
		vuelta = "";
		break;
	}
	return vuelta;
}

std::string ContainerNodeType::getTypeImagePath() {
	std::string prefix = getMovementPrefix();
	std::string subfix = getContainerSubfix();
	return PATH_TO_IMGS + prefix + subfix;
}

std::string ContainerNodeType::getMovementPrefix() {
	std::string vuelta;
	switch (this->movementType) {
	case MovementType::continuous:
		vuelta = CONTINUOUS_PREFIX;
		break;
	case MovementType::discrete:
		vuelta = DISCRETE_PREFIX;
		break;
	case MovementType::irrelevant:
		vuelta = IRRELEVANT_PREFIX;
		break;
	default:
		vuelta = "";
		break;
	}
	return vuelta;
}

std::string ContainerNodeType::getContainerSubfix() {
	std::string vuelta;
	switch (this->containerType) {
	case ContainerType::inlet:
		vuelta = INLET_IMG;
		break;
	case ContainerType::flow:
		vuelta = FLOW_IMG;
		break;
	case ContainerType::sink:
		vuelta = SINK_IMG;
		break;
	case ContainerType::divergent_switch:
		vuelta = DIVERGENT_SWITCH_IMG;
		break;
	case ContainerType::convergent_switch:
		vuelta = CONVERGENT_SWITCH_IMG;
		break;
	case ContainerType::divergent_switch_sink:
		vuelta = DIVERGENT_SWITCH_SINK_IMG;
		break;
	case ContainerType::convergent_switch_inlet:
		vuelta = CONV_SWITCH_INLET_IMG;
		break;
	case ContainerType::bidirectional_switch:
		vuelta = BIDIRECTIONAL_SWITCH_IMG;
		break;
	case ContainerType::unknow:
		vuelta = UNKNOW_IMG;
		break;
	}
	return vuelta;
}

int ContainerNodeType::calculateHashCode()
{
	int containerHash = (int) this->containerType;
	int containerDigits = ceil(((int)ContainerType::CONTAINER_MAX) / 10.0);

	int movementHash = ((int)this->movementType) * pow(10.0, containerDigits);
	int movementDigits = ceil(((int)MovementType::MOVEMENT_MAX) / 10.0);

	int addonsHash = 0;
	for (int i = 0; i < AddOnsType::ADDONS_MAX; i++) {
		if (this->addOns[i]) {
			addonsHash += (1 << i);
		}
	}
	addonsHash = addonsHash * pow(10.0, containerDigits + movementDigits);

	return addonsHash + movementHash + containerHash;

}

bool ContainerNodeType::operator ==(const ContainerNodeType& n1) {
	bool equals = (n1.movementType == this->movementType) && (n1.containerType == this->containerType);
	for (int i = 0; equals && (i < AddOnsType::ADDONS_MAX); i++) {
		equals = (n1.addOns[i] == this->addOns[i]);
	}
	return equals;
}
