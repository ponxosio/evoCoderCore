/*
 * ExecutableContainerNode.h
 *
 *  Created on: 3 de may. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_EXECUTABLECONTAINERNODE_H_
#define SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_EXECUTABLECONTAINERNODE_H_

#include <string>
#include <stdexcept>

#include "../../machineGraph/ContainerNode.h"
#include "../../machineGraph/ContainerNodeType.h"
#include "actuators/extras/Light.h"
#include "actuators/extras/Mixer.h"
#include "actuators/extras/ODSensor.h"
#include "actuators/extras/Temperature.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

class ExecutableContainerNode: public ContainerNode {
public:
	//Obligatory methods if is derived from NODE
	ExecutableContainerNode() :
			ContainerNode() {
		this->od = std::shared_ptr<ODSensor>();
		this->mix = std::shared_ptr<Mixer>();
		//this->light = NULL;
		this->light = std::shared_ptr<Light>();
		this->temperature = std::shared_ptr<Temperature>();
	}
	ExecutableContainerNode(const ExecutableContainerNode & node) :
			ContainerNode(node) {
		this->od = node.od;
		this->mix = node.mix;
		//this->light = NULL;
		this->light = node.light;
		this->temperature = node.temperature;
	}
	//

	ExecutableContainerNode(int idConatiner, std::shared_ptr<ContainerNodeType> type, float capacity) :
			ContainerNode(idConatiner, type, capacity) {
		this->od = std::shared_ptr<ODSensor>();
		this->mix = std::shared_ptr<Mixer>();
		//this->light = NULL;
		this->light = std::shared_ptr<Light>();
		this->temperature = std::shared_ptr<Temperature>();
	}
	virtual ~ExecutableContainerNode(){}

	inline virtual std::string toText() {
		return ContainerNode::toText();
	}

	//getters & setters
	inline const std::shared_ptr<Light>& getLight() const {
		return light;
	}

	inline void setLight(const std::shared_ptr<Light>& light) {
		addAddon(AddOnsType::light);
		this->light = light;
	}

	inline const std::shared_ptr<Mixer>& getMix() const {
		return mix;
	}

	inline void setMix(const std::shared_ptr<Mixer>& mix) {
		addAddon(AddOnsType::mixer);
		this->mix = mix;
	}

	inline const std::shared_ptr<ODSensor>& getOd() const {
		return od;
	}

	inline void setOd(const std::shared_ptr<ODSensor>& od) {
		addAddon(AddOnsType::OD_sensor);
		this->od = od;
	}

	inline const std::shared_ptr<Temperature>& getTemperature() const {
		return temperature;
	}

	inline void setTemperature(
			const std::shared_ptr<Temperature>& temperature) {
		addAddon(AddOnsType::temp);
		this->temperature = temperature;
	}

	//virtual methods
	virtual void setPositionInject(int source, int target) throw (std::runtime_error) = 0;
	virtual void setPositionExtract(int source, int target) throw (std::runtime_error) = 0;
	virtual void receiveLiquid(double rate) throw (std::runtime_error) = 0;
	virtual void extractLiquid(double rate) throw (std::runtime_error)= 0;
	virtual void connectContainer(int source, int target) throw (std::runtime_error) = 0;
	virtual void clearConnectedContainers() throw (std::runtime_error) = 0;
	
	virtual void updateCommunicationInterface(int communication) = 0;

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	//Light* light;
	std::shared_ptr<ODSensor> od;
	std::shared_ptr<Mixer> mix;
	std::shared_ptr<Light> light;
	std::shared_ptr<Temperature> temperature;
};

template<class Archive>
inline void ExecutableContainerNode::serialize(Archive& ar,
		const std::uint32_t version) {
	if (version <= 1) {
		ContainerNode::serialize(ar, version);
		ar(CEREAL_NVP(od), CEREAL_NVP(mix), CEREAL_NVP(light), CEREAL_NVP(temperature));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( ExecutableContainerNode, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(ExecutableContainerNode, "ExecutableContainerNode");

#endif /* SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_EXECUTABLECONTAINERNODE_H_ */
