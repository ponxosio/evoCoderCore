/*
 * Mix.h
 *
 *  Created on: 23 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_MIX_H_
#define SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_MIX_H_

//boost
#include <memory>

//local
#include "../../../operables/mathematics/MathematicOperable.h"
#include "ContainerOperation.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

class Mix: public ContainerOperation {
public:
	// Node methods
	Mix();
	Mix(const Mix & node);

	virtual std::string toText();
	virtual void loadNode(const std::string & line) throw (invalid_argument);
	//

	Mix(int idConatiner, int idSource1,
			int idSource2, int idTarget,
			std::shared_ptr<MathematicOperable> volume1,
			std::shared_ptr<MathematicOperable> volume2);

	virtual ~Mix();

	virtual void execute() throw(std::invalid_argument);

	virtual inline void updateReference(const std::string & reference)
	{
		ContainerOperation::updateReference(reference);
		volume1->updateReference(reference);
		volume2->updateReference(reference);
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	int idSource1;
	int idSource2;
	int idTarget;
	std::shared_ptr<MathematicOperable> volume1;
	std::shared_ptr<MathematicOperable> volume2;
};

template<class Archive>
inline void Mix::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		ContainerOperation::serialize(ar, version);
		ar(CEREAL_NVP(idSource1),CEREAL_NVP(idSource2), CEREAL_NVP(idTarget), CEREAL_NVP(volume1), CEREAL_NVP(volume2));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( Mix, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(Mix,"Mix");

#endif /* SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_CONTAINER_MIX_H_ */
