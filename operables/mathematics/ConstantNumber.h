/*
 * ConstantNumber.h
 *
 *  Created on: 15 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_OPERABLES_MATHEMATICS_CONSTANTNUMBER_H_
#define SRC_OPERABLES_MATHEMATICS_CONSTANTNUMBER_H_

#include <memory>

#include "../../util/Utils.h"
#include "../../util/Patch.h"
#include "MathematicOperable.h"

 //cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>

/**
 * Represents a constant number.
 */
class ConstantNumber: public MathematicOperable {
public:
	ConstantNumber();
	ConstantNumber(int value);
	ConstantNumber(double value);
	virtual ~ConstantNumber(){};

	virtual double getValue();
	virtual bool equal ( const MathematicOperable* obj ) const;

	inline virtual bool isPhysical() {return false;}

	inline virtual std::string toString() {
		return patch::to_string(value);
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	double value;
};

template<class Archive>
inline void ConstantNumber::serialize(Archive& ar,
		const std::uint32_t version) {
	if (version == 1) {
		ar(CEREAL_NVP(value));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( ConstantNumber, 1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(ConstantNumber, "ConstantNumber");

#endif /* SRC_OPERABLES_MATHEMATICS_CONSTANTNUMBER_H_ */
