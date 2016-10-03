/*
 * ApplyLight.h
 *
 *  Created on: 28 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_APPLYLIGHT_H_
#define SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_APPLYLIGHT_H_

//boost
#include "boost/shared_ptr.hpp"

//local
#include "../../../operables/mathematics/MathematicOperable.h"
#include "ContainerOperation.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

#include "evocodercore_global.h"

class /*APPLYLIGHT_EXPORT*/ ApplyLight: public ContainerOperation {
public:
	// Node methods
	ApplyLight();
	ApplyLight(const ApplyLight & node);

	virtual std::string toText();
	virtual void loadNode(const std::string & line)
			throw (std::invalid_argument);
	//
	ApplyLight(int idContainer,
			int sourceID, std::shared_ptr<MathematicOperable> wavelength,
			std::shared_ptr<MathematicOperable> intensity);

	virtual ~ApplyLight();

	virtual void execute() throw(std::invalid_argument);

	virtual inline void updateReference(const std::string & reference) 
	{
		ContainerOperation::updateReference(reference);
		wavelength->updateReference(reference);
		intensity->updateReference(reference);
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	int sourceId;
	std::shared_ptr<MathematicOperable> wavelength;
	std::shared_ptr<MathematicOperable> intensity;
};

template<class Archive>
inline void ApplyLight::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		ContainerOperation::serialize(ar, version);
		ar(CEREAL_NVP(sourceId), CEREAL_NVP(wavelength), CEREAL_NVP(intensity));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( ApplyLight, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(ApplyLight,"ApplyLight");

#endif /* SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_APPLYLIGHT_H_ */
