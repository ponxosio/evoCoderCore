/*
 * Control.h
 *
 *  Created on: 3 de may. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_EXECUTABLEMACHINEGRAPH_CONTAINERS_ACTUATORSINTERFACES_LIQUIDACTUATORS_CONTROL_H_
#define SRC_FLUIDCONTROL_EXECUTABLEMACHINEGRAPH_CONTAINERS_ACTUATORSINTERFACES_LIQUIDACTUATORS_CONTROL_H_

#include <stdexcept>

#include "../Instructable.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

class Control: public Instructable {
public:
	Control() {
		this->communications = -1;
	}
    Control(int communications) :
			Instructable() {
		this->communications = communications;
	}
	virtual ~Control(){}

	virtual void addConnection(int idSource, int idTarget) throw (std::runtime_error) = 0;
	virtual void setConnection(int idSource, int idTarget) throw (std::runtime_error) = 0;
	virtual void clearConnections() throw (std::runtime_error) = 0;
	virtual std::string getInstructions() throw (std::runtime_error) = 0;

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
inline void Control::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
        ar(CEREAL_NVP(communications));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( Control, (int)2 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(Control, "Control");

#endif /* SRC_FLUIDCONTROL_EXECUTABLEMACHINEGRAPH_CONTAINERS_ACTUATORSINTERFACES_LIQUIDACTUATORS_CONTROL_H_ */
