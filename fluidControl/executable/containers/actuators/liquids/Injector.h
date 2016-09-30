/*
 * Injector.h
 *
 *  Created on: 3 de may. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_EXECUTABLEMACHINEGRAPH_CONTAINERS_ACTUATORSINTERFACES_LIQUIDACTUATORS_INJECTOR_H_
#define SRC_FLUIDCONTROL_EXECUTABLEMACHINEGRAPH_CONTAINERS_ACTUATORSINTERFACES_LIQUIDACTUATORS_INJECTOR_H_

#include <stdexcept>

#include "../Instructable.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

class Injector: public Instructable {
public:
	Injector() {
		this->communications = -1;
	}
	Injector(int communications) : Instructable() {
		this->communications = communications;
	}
	virtual ~Injector(){}

	virtual void injectLiquid(double rate) throw(std::runtime_error) = 0;
	virtual std::string getInstructions() throw (std::runtime_error) = 0;
	virtual int getMovementType() throw (std::runtime_error) = 0;

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
inline void Injector::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		ar(CEREAL_NVP(communications));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( Injector, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(Injector, "Injector");

#endif /* SRC_FLUIDCONTROL_EXECUTABLEMACHINEGRAPH_CONTAINERS_ACTUATORSINTERFACES_LIQUIDACTUATORS_INJECTOR_H_ */
