/*
 * VariableEntry.h
 *
 *  Created on: 15 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_OPERABLES_MATHEMATICS_VARIABLEENTRY_H_
#define SRC_OPERABLES_MATHEMATICS_VARIABLEENTRY_H_

#pragma warning( disable : 4290 )

#include <string>
#include <memory>
#include <stdexcept>

//local
#include "util/Utils.h"
#include "operables/VariableTable.h"
#include "operables/mathematics/MathematicOperable.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>

#include "evocodercore_global.h"

/**
 * Represents a variable stored in the VariableTable.
 */
class VARIABLEENTRY_EXPORT VariableEntry: public MathematicOperable {
public:
	VariableEntry();
	VariableEntry(const std::string & name);
	/**
	 * Does not deallocates the sharedTable
	 */
	virtual ~VariableEntry() {}

	virtual void updateReference(const std::string & reference);

	/**
	 * Returns the value from the variable table
	 * @return
	 */
	virtual double getValue() throw (std::invalid_argument);
	/**
	 * Check if the variable stores physical values
	 * @return true if the variable stores physical values, false otherwise
	 */
	virtual bool isPhysical() throw (std::invalid_argument);

	virtual bool equal(const MathematicOperable* obj) const;
	/**
	 * Sets the value of this variable at the variable table to the given value
	 * @param value
	 */
	void setValue(double value) throw (std::invalid_argument);
	/**
	 * Sets the physical flag of this variable at the variable table to the given value
	 * @param value
	 */
	void setPhysical(bool physical) throw (std::invalid_argument);

	inline virtual std::string toString() {
		return name;
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	/**
	 * name of the variable in the table
	 */
	std::string name;
	/**
	 * On delete does not deallocate this pointer to the variable table
	 */
	std::string reference;

    std::shared_ptr<VariableTable> getVariableTable() throw (std::invalid_argument);
};

template<class Archive>
inline void VariableEntry::serialize(Archive& ar, const std::uint32_t version) {
	if (version == 1) {
		ar(CEREAL_NVP(name), CEREAL_NVP(reference));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( VariableEntry, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(VariableEntry, "VariableEntry");

#endif /* SRC_OPERABLES_MATHEMATICS_VARIABLEENTRY_H_ */
