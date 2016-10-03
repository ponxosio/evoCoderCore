/*
 * Transfer.h
 *
 *  Created on: 23 de mar. de 2016
 *      Author: angel
 */

#pragma once

#pragma warning( disable : 4290 )

#include <string>

//local
#include "operables/mathematics/MathematicOperable.h"
#include "ContainerOperation.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

#include "evocodercore_global.h"

class TRANSFER_EXPORT Transfer: public ContainerOperation {
public:
	//Node methods
	Transfer();
	Transfer(const Transfer & node) ;
	virtual ~Transfer();

	virtual std::string toText();
	virtual void loadNode(const std::string & line) throw (std::invalid_argument);
	//

	Transfer(int idContainer, int idSource,
			int idTarget, std::shared_ptr<MathematicOperable> volume);

	virtual void execute() throw(std::invalid_argument);

	virtual inline void updateReference(const std::string & reference)
	{
		ContainerOperation::updateReference(reference);
		volume->updateReference(reference);
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	int idSource;
	int idTarget;
	std::shared_ptr<MathematicOperable> volume;
};

template<class Archive>
inline void Transfer::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		ContainerOperation::serialize(ar, version);
		ar(CEREAL_NVP(idSource), CEREAL_NVP(idTarget), CEREAL_NVP(volume));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( Transfer,(int) 1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(Transfer,"Transfer");
