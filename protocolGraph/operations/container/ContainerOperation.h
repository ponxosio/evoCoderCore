/*
 * ContainerOperation.h
 *
 *  Created on: 23 de mar. de 2016
 *      Author: angel
 */

#pragma once

#include <string>
#include <stdexcept>

//local
#include "../../../ExecutionServer.h"
#include "../../../util/Utils.h"
#include "../../../operables/mathematics/MathematicOperable.h"
#include "../../OperationNode.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>

#include "evocodercore_global.h"

/**
 * Interface that represents all the operations that can be made over the machine containers
 */
class /*CONTAINEROPERATION_EXPORT*/ ContainerOperation: public OperationNode {
public:
	ContainerOperation() : OperationNode() {}
	ContainerOperation(const ContainerOperation & obj) : OperationNode(obj) {}
	ContainerOperation(int idConatiner) : OperationNode(idConatiner) {}

	virtual ~ContainerOperation(){}
	
	virtual inline void updateReference(const std::string & reference) {
		this->reference = reference;
	}

	virtual inline std::shared_ptr<Mapping> getMapping() throw (std::invalid_argument) {
		try {
			return ExecutionServer::GetInstance()->getEvoCoder(reference)->getMapping();
		}
		catch (std::invalid_argument & e) {
			throw(std::invalid_argument("ContainerOperation::getMapping(), " + std::string(e.what())));
		}
	}

	//pure virtual
	virtual void execute() throw(std::invalid_argument) = 0;
	
	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);

protected:
	std::string reference;
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
