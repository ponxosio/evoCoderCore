/*
 * ContainerNodeType.h
 *
 *  Created on: 4 de may. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_MACHINEGRAPH_CONTAINERNODETYPE_H_
#define SRC_FLUIDCONTROL_MACHINEGRAPH_CONTAINERNODETYPE_H_

//NAME CONSTANTS
//movements
#define CONTINUOUS_NAME "continuous"
#define DISCRETE_NAME "discrete"
#define IRRELEVANT_NAME "irrelevant"
//containers
#define INLET_NAME "inlet"
#define FLOW_NAME "flow"
#define SINK_NAME "sink"
#define DIVERGENT_SWITCH_NAME "divergent_switch"
#define CONVERGENT_SWITCH_NAME "convergent_switch"
#define BIDIRECTIONAL_SWITCH_NAME "bidirectional_switch"
#define CONV_SWITCH_INLET_NAME "convergent_switch_inlet"
#define DIVERGENT_SWITCH_SINK_NAME "divergent_switch_sink"
#define UNKNOW_NAME "unknown"

//PATH CONSTANTS
#define PATH_TO_IMGS "\\Img\\"
//movements
#define CONTINUOUS_PREFIX "continuous_"
#define DISCRETE_PREFIX "discrete_"
#define IRRELEVANT_PREFIX ""
//containers
#define INLET_IMG "inlet.png"
#define FLOW_IMG "flow.png"
#define SINK_IMG "sink.png"
#define DIVERGENT_SWITCH_IMG "divergentswitch.png"
#define CONVERGENT_SWITCH_IMG "convergentswitch.png"
#define BIDIRECTIONAL_SWITCH_IMG "bidirectionalswitch.png"
#define CONV_SWITCH_INLET_IMG "convergentswitchinlet.png"
#define DIVERGENT_SWITCH_SINK_IMG "divergentswitchsink.png"
#define UNKNOW_IMG "unknow.png"
//addons
#define OD_SENSOR "odsensor.png"
#define TEMPERATURE "temperature.png"
#define LIGHT "light.png"
#define MIXER "mixer.png"

#include <string>
#include  <stdexcept>

//data structures
#include <vector>
#include <array>

//local
#include "../../util/Patch.h"
#include "../../util/Utils.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/array.hpp>


/*** Enum for the type of fluid movement ***/
typedef enum MovementType_ {
	continuous,	// chamber with peristaltic pump or similar
	discrete,	// chamber with syringe pump or similar
	irrelevant, //chamber with no pump
	MOVEMENT_MAX = irrelevant + 1 //Max number ALWAYS EQUALS TO THE SECOND-TO-LAST TYPE PLUS ONE
} MovementType;

/*** Enum for the type of container ***/
typedef enum ContainerType_ {
	inlet, //container capable of sending liquid to an other container
	flow, //container capable of sending and receiving liquid from/to an other container
	sink, //container capable of receiving liquid from an other container
	divergent_switch, //container capable of sending liquid to several other containers
	convergent_switch, //container capable of receiving liquid from several other containers
	divergent_switch_sink, //container capable of sending liquid to several other containers and receive liquid from one
	convergent_switch_inlet, //container capable of receiving liquid from several other containers and send liquid to one
	bidirectional_switch, //container capable of sending/receiving liquid to/from several other containers
	unknow, //passive element
	CONTAINER_MAX = unknow + 1 //Max number ALWAYS EQUALS TO THE SECOND-TO-LAST TYPE PLUS ONE
} ContainerType;

/*** Enum for the type of AddOns ***/
typedef enum AddOnsType_ {
	mixer,	// addon that stir a liquid
	light,	// addon that apply light
	temp,  // addon that heat/cold a liquid
	OD_sensor, // addon that reads Optical density
	ADDONS_MAX = OD_sensor + 1 // Max number of Addons ALWAYS EQUALS TO THE SECOND-TO-LAST TYPE PLUS ONE
} AddOnsType;


class ContainerNodeType {
public:
	static MovementType fromIntToMovementType(int movement);

	ContainerNodeType();
	ContainerNodeType(const ContainerNodeType & type);
	ContainerNodeType(MovementType movementType, ContainerType containerType);
	ContainerNodeType(MovementType movementType, ContainerType containerType, std::vector<AddOnsType> addOns);
	virtual ~ContainerNodeType();

	void changeContainerType(ContainerType newContainerType);
	void changeMovementType(MovementType newMovementType);
	void addAddon(AddOnsType addOn);
	void removeAddon(AddOnsType addOn);

	bool isCompatible(const ContainerNodeType & type);
	bool isCompatibleMovement(MovementType movement);
	bool hasMovementType(MovementType movement);
	bool hasContainerType(ContainerType container);
	bool hasAddOns(AddOnsType addOn);
	int calculateHashCode();

	std::string getTypeString();
	std::string getContainerTypeString();
	std::string getMovementTypeString();
	void load (const std::string & textType) throw (std::invalid_argument);
	std::string getTypeImagePath();
	std::string makeAddonsNodes(int idContainer);

	inline MovementType getMovementType() const {
		return movementType;
	}

	inline ContainerType getContainerType() const {
		return containerType;
	}

	bool operator == (const ContainerNodeType& n1);

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	MovementType movementType;
	ContainerType containerType;
	std::array<bool, AddOnsType::ADDONS_MAX> addOns;

	bool isCompatibleContainer(ContainerType container);
	bool isCompatibleAddOns(const std::array<bool, AddOnsType::ADDONS_MAX> & addOns);

	std::string getMovementPrefix();
	std::string getContainerSubfix();
	std::string getAddonsImg(AddOnsType addonType);
};

template<class Archive>
inline void ContainerNodeType::serialize(Archive& ar,
		const std::uint32_t version) {
	if (version <= 1) {
		ar(CEREAL_NVP(movementType), CEREAL_NVP(containerType), CEREAL_NVP(addOns));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( ContainerNodeType, (int)1 );

#endif /* SRC_FLUIDCONTROL_MACHINEGRAPH_CONTAINERNODETYPE_H_ */
