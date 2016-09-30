/*
 * Tautology.h
 *
 *  Created on: 28 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_OPERABLES_COMPARISON_TAUTOLOGY_H_
#define SRC_OPERABLES_COMPARISON_TAUTOLOGY_H_

#include "ComparisonOperable.h"
#include "../../util/Utils.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

class Tautology: public ComparisonOperable {
public:
	Tautology() {
		this->negation = false;
	}
	virtual ~Tautology() {
	}

	/**
	 * Check if the comparison is true o false
	 * @return true if the comparison is true, false otherwise
	 */
	inline virtual bool conditionMet() {
		return !negation;
	}
	/**
	 * Check if the comparison used physical values
	 * @return true if the comparison uses physical values, false otherwise
	 */
	inline virtual bool isPhysical() {
		return false;
	}
	/**
	 * Checks if two ComparisonOperable are the same
	 * @param obj other ComparisonOperable to be compared
	 * @return true if they are the same, false otherwise
	 */
	inline virtual bool equal(ComparisonOperable* obj) const {
		bool vuelta = false;
		if (Utils::IsType<Tautology, ComparisonOperable>(obj)) {
			vuelta = true;
		}
		return vuelta;
	}
	/**
	 * !(tautology)
	 */
	inline virtual void negate() {
		negation = !negation;
	}

	inline virtual std::string toString() {
		std::string neg = negation ? "!" : "";
		return neg + "true";
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	bool negation;
};

template<class Archive>
inline void Tautology::serialize(Archive& ar, const std::uint32_t version) {
	if (version == 1) {
		ar(CEREAL_NVP(negation));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( Tautology, 1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(Tautology, "Tautology");

#endif /* SRC_OPERABLES_COMPARISON_TAUTOLOGY_H_ */
