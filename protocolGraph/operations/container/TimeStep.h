/*
 * TimeStep.h
 *
 *  Created on: 28 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_TIMESTEP_H_
#define SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_TIMESTEP_H_

//boost
#include <memory>

//local
#include "../../../operables/mathematics/VariableEntry.h"
#include "ContainerOperation.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

#include "evocodercore_global.h"

class TIMESTEP_EXPORT TimeStep: public ContainerOperation {
public:
	// Node methods
	TimeStep();
	TimeStep(const TimeStep & node);

	virtual std::string toText();
	virtual void loadNode(const std::string & line) throw (invalid_argument);
	//
	TimeStep(int containerId, std::shared_ptr<VariableEntry> receiver);

	virtual ~TimeStep();

	virtual void execute() throw(std::invalid_argument);

	virtual inline void updateReference(const std::string & reference)
	{
		ContainerOperation::updateReference(reference);
		receiver->updateReference(reference);
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	std::shared_ptr<VariableEntry> receiver;
};

template<class Archive>
inline void TimeStep::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		ContainerOperation::serialize(ar, version);
		ar(CEREAL_NVP(receiver));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( TimeStep, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(TimeStep,"TimeStep");

#endif /* SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_TIMESTEP_H_ */
