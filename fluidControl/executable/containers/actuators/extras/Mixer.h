/*
 * Mixer.h
 *
 *  Created on: 11 de may. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_ACTUATORS_EXTRAS_MIXER_H_
#define SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_ACTUATORS_EXTRAS_MIXER_H_

#pragma warning( disable : 4290 )

#include <stdexcept>

#include "fluidControl/executable/containers/actuators/Instructable.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

#include "evocodercore_global.h"

class MIXER_EXPORT Mixer: public Instructable {
public:
	Mixer() {
		this->communications = -1;
	}
	Mixer(int communications) :
			Instructable() {
		this->communications = communications;
	}
	virtual ~Mixer(){}

	virtual std::string getInstructions() throw (std::runtime_error) = 0;
	virtual void mix(double intensity) throw(std::runtime_error) = 0;

	//getters and setters
	inline void setCommunications(int communications) {
		this->communications = communications;
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	int communications;
};

template<class Archive>
inline void Mixer::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		ar(CEREAL_NVP(communications));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( Mixer, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(Mixer, "Mixer");

#endif /* SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_ACTUATORS_EXTRAS_MIXER_H_ */
