/*
 * InletContainer.h
 *
 *  Created on: 3 de may. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_INLETCONTAINER_H_
#define SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_INLETCONTAINER_H_

//boost
#include <memory>

//local
#include "../../executable/containers/ExecutableContainerNode.h"
#include "actuators/liquids/Extractor.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

class InletContainer: public ExecutableContainerNode {
public:
	//methods of being node
	InletContainer();
	InletContainer(const InletContainer & node);

	//virtual std::string toText();
	virtual void loadNode(const std::string & line)
			throw (std::invalid_argument);
	//
	InletContainer(int idConatiner, float capacity, std::shared_ptr<Extractor> ext);

	virtual ~InletContainer();

	virtual void setPositionInject(int source, int target) throw (std::runtime_error);
	virtual void setPositionExtract(int source, int target) throw (std::runtime_error);
	virtual void receiveLiquid(double rate) throw (std::runtime_error);
	virtual void extractLiquid(double rate) throw (std::runtime_error);
	virtual void connectContainer(int source, int target) throw (std::runtime_error);
	inline virtual void clearConnectedContainers() throw (std::runtime_error) {}

	virtual void updateCommunicationInterface(int communication);

    //getters & setters
    inline std::shared_ptr<Extractor> getExtractor() {
        return ext;
    }

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	std::shared_ptr<Extractor> ext;
};

template<class Archive>
inline void InletContainer::serialize(Archive& ar,
		const std::uint32_t version) {
	if (version <= 1) {
		ExecutableContainerNode::serialize(ar, version);
		ar(CEREAL_NVP(ext));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( InletContainer, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(InletContainer, "InletContainer");

#endif /* SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_INLETCONTAINER_H_ */
