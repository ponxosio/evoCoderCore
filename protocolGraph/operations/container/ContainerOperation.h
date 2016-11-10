/*
 * ContainerOperation.h
 *
 *  Created on: 23 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_CONTAINEROPERATION_H_
#define SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_CONTAINEROPERATION_H_

#pragma warning( disable : 4290 )

#include <string>
#include <stdexcept>

//local
#include "util/Utils.h"
#include "operables/mathematics/MathematicOperable.h"
#include "protocolGraph/OperationNode.h"
#include "fluidControl/mapping/Mapping.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>

#include "evocodercore_global.h"

/**
 * Interface that represents all the operations that can be made over the machine containers
 */
class CONTAINEROPERATION_EXPORT ContainerOperation: public OperationNode {
public:
    ContainerOperation();
    ContainerOperation(const ContainerOperation & obj);
    ContainerOperation(int idConatiner);

    virtual ~ContainerOperation();

    //overriden
    virtual void updateReference(const std::string & reference);
	//pure virtual
	virtual void execute() throw(std::invalid_argument) = 0;
	
	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);

protected:
	std::string reference;

    std::shared_ptr<Mapping> getMapping() throw (std::invalid_argument);
};

template<class Archive>
inline void ContainerOperation::serialize(Archive& ar,
		const std::uint32_t version) {
	if (version <= 1) {
		OperationNode::serialize(ar, version);
		ar(CEREAL_NVP(reference));
	}
}
// Associate some type with a version number
CEREAL_CLASS_VERSION( ContainerOperation, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(ContainerOperation,"ContainerOperation");

#endif /* SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_CONTAINEROPERATION_H_ */
