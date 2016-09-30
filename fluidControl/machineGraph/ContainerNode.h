/*
 * ContainerNode.h
 *
 *  Created on: 8 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_MACHINEGRAPH_CONTAINERNODE_H_
#define SRC_FLUIDCONTROL_MACHINEGRAPH_CONTAINERNODE_H_

#include <string>
#include  <stdexcept>
#include <iostream>

//boost
#include <boost/algorithm/string/trim.hpp>
#include <memory>

//local
#include "../../util/Patch.h"
#include "../../util/Utils.h"
#include "../../graph/Node.h"
#include "ContainerNodeType.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

class ContainerNode: public Node {

public:
	//Obligatory constructors if is dereived from NODE
	ContainerNode();
	ContainerNode(const ContainerNode & node);
	//
	ContainerNode(int idConatiner, std::shared_ptr<ContainerNodeType> type, float capacity);

	virtual ~ContainerNode();

	//ContainerType methods
	void changeContainerType(ContainerType type);
	void changeMovementType(MovementType type);
	void addAddon(AddOnsType type);
	void removeAddon(AddOnsType type);

	//getters and setters
	inline std::shared_ptr<ContainerNodeType> getType() {
		return type;
	}
	inline void setType(std::shared_ptr<ContainerNodeType> type) {
		this->type = type;
	}

	inline float getVolume() {
		return volume;
	}
	inline void setVolume(float volume) {
		this->volume = volume;
	}

	inline float getCapacity() {
		return capacity;
	}
	inline void setCapacity(float capacity) {
		this->capacity = capacity;
	}

	//overriden virtual methods
	/**
	 * Returns a string with all the information of the node, must be with a format that loadNode can manage
	 *
	 * @return a string with all the necessary information to store this node
	 */
	virtual std::string toText();

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	/**
	 * Container type
	 */
	std::shared_ptr<ContainerNodeType> type;
	/**
	 * Maximum volume capacity of the container
	 */
	float capacity;
	/**
	 * Actual volume of the liquid in the container
	 */
	float volume;
};

template<class Archive>
inline void ContainerNode::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		Node::serialize(ar, version);
		ar(CEREAL_NVP(type), CEREAL_NVP(capacity), CEREAL_NVP(volume));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( ContainerNode, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(ContainerNode,"ContainerNode");

#endif /* SRC_FLUIDCONTROL_MACHINEGRAPH_CONTAINERNODE_H_ */
