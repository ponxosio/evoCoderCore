/*
 * ApplyLight.cpp
 *
 *  Created on: 28 de mar. de 2016
 *      Author: angel
 */

#include "ApplyLight.h"

ApplyLight::ApplyLight() : ContainerOperation() {
	this->sourceId = -1;
	this->wavelength = std::shared_ptr<MathematicOperable>();
	this->intensity = std::shared_ptr<MathematicOperable>();
}

ApplyLight::ApplyLight(const ApplyLight& node) : ContainerOperation(node) {
	this->sourceId = node.sourceId;
	this->wavelength = node.wavelength;
	this->intensity = node.intensity;
}

std::string ApplyLight::toText() {
	return patch::to_string(containerID) + "[label=\"applyLigth("
			+ patch::to_string(sourceId) + ", " + wavelength.get()->toString()
			+ ", " + intensity.get()->toString() + ")\"];";
}

void ApplyLight::loadNode(const std::string& line) throw (std::invalid_argument) {
	//TODO: JSON
}

ApplyLight::ApplyLight(int idContainer, int sourceID,
		std::shared_ptr<MathematicOperable> wavelength,
		std::shared_ptr<MathematicOperable> intensity) :
	ContainerOperation(idContainer){
	this->sourceId = sourceID;
	this->wavelength = wavelength;
	this->intensity = intensity;
}

ApplyLight::~ApplyLight() {

}

void ApplyLight::execute() throw(std::invalid_argument)  {
	ContainerOperation::getMapping()->applyLight(sourceId, wavelength.get()->getValue(), intensity.get()->getValue());
}
