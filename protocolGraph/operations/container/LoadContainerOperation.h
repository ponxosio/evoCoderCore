/*
 * LoadContainer.h
 *
 *  Created on: 5 de abr. de 2016
 *      Author: angel
 */

#pragma once

#pragma warning( disable : 4290 )

//local
#include "util/Utils.h"
#include "operables/mathematics/MathematicOperable.h"
#include "ContainerOperation.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

#include "evocodercore_global.h"

class LOADCONTAINEROPERATION_EXPORT LoadContainerOperation: public ContainerOperation {
public:
	//methods for node
	LoadContainerOperation();
	LoadContainerOperation(const LoadContainerOperation & obj);

	virtual std::string toText();
	virtual void loadNode(const std::string & line)	throw (std::invalid_argument);
	//
	LoadContainerOperation(int idConatiner, int idSource, std::shared_ptr<MathematicOperable> value);

	virtual ~LoadContainerOperation();

	virtual void execute() throw(std::invalid_argument);

	virtual inline void updateReference(const std::string & reference)
	{
		ContainerOperation::updateReference(reference);
		value->updateReference(reference);
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	int idSource;
	std::shared_ptr<MathematicOperable> value;
};

template<class Archive>
inline void LoadContainerOperation::serialize(Archive& ar,
		const std::uint32_t version) {
	if (version <= 1) {
		ContainerOperation::serialize(ar, version);
		ar(CEREAL_NVP(idSource), CEREAL_NVP(value));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( LoadContainerOperation, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(LoadContainerOperation,"LoadContainerOperation");
