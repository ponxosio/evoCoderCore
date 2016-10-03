/*
 * ApplyTemperature.h
 *
 *  Created on: 28 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_APPLYTEMPERATURE_H_
#define SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_APPLYTEMPERATURE_H_

#pragma warning( disable : 4290 )

//local
#include "operables/mathematics/MathematicOperable.h"
#include "ContainerOperation.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

#include "evocodercore_global.h"

class APPLYTEMPERATURE_EXPORT ApplyTemperature: public ContainerOperation {
public:
	// Node methods
	ApplyTemperature();
	ApplyTemperature(const ApplyTemperature & node);

	virtual std::string toText();
	virtual void loadNode(const std::string & line)
			throw (std::invalid_argument);
	//
	ApplyTemperature(int containerId, int sourceId,
			std::shared_ptr<MathematicOperable> degress);

	virtual ~ApplyTemperature();

	virtual void execute() throw(std::invalid_argument);

	virtual inline void updateReference(const std::string & reference)
	{
		ContainerOperation::updateReference(reference);
		degress->updateReference(reference);
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	int sourceId;
	std::shared_ptr<MathematicOperable> degress;
};

template<class Archive>
inline void ApplyTemperature::serialize(Archive& ar,
		const std::uint32_t version) {
	if (version <= 1) {
		ContainerOperation::serialize(ar, version);
		ar(CEREAL_NVP(sourceId), CEREAL_NVP(degress));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( ApplyTemperature, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(ApplyTemperature,"ApplyTemperature");
#endif /* SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_APPLYTEMPERATURE_H_ */
