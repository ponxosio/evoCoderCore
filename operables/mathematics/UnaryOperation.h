/*
 * UnaryOperation.h
 *
 *  Created on: 4 de abr. de 2016
 *      Author: angel
 */

#ifndef SRC_OPERABLES_MATHEMATICS_UNARYOPERATION_H_
#define SRC_OPERABLES_MATHEMATICS_UNARYOPERATION_H_

#define ABSOLUTE_VALUE_STRING "fabs"
#define FLOOR_VALUE_STRING "floor"

//boost
#include <memory>

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>

//local
#include "../../util/Utils.h"
#include "MathematicOperable.h"

namespace unaryOperations {
/*** Enum for the type of unary operator ***/
enum UnaryOperator {
	absoluteValue, //abs()
	floor  
};
}
class UnaryOperation: public MathematicOperable {
public:
	UnaryOperation();
	UnaryOperation(std::shared_ptr<MathematicOperable> variable,
			unaryOperations::UnaryOperator op);

	virtual ~UnaryOperation();

	virtual void updateReference(const std::string & reference);

	/**
	 * Returns the numeric value of the variable
	 * @return the numeric value of the variable
	 */
	virtual double getValue() throw (std::invalid_argument);
	/**
	 * Check if the variable stores physical values
	 * @return true if the variable stores physical values, false otherwise
	 */
	virtual bool isPhysical() throw (std::invalid_argument);
	/**
	 * Compares two Objects implementing this interface
	 * @param obj other object to be compared to
	 * @return true if both are the same, false otherwise.
	 */
	virtual bool equal(const MathematicOperable* obj) const;

	inline virtual std::string toString() {
		return getStringOp() + "(" + variable.get()->toString() + ")";
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);

protected:
	std::shared_ptr<MathematicOperable> variable;
	unaryOperations::UnaryOperator op;

	std::string getStringOp();
};

template<class Archive>
inline void UnaryOperation::serialize(Archive& ar,
		const std::uint32_t version) {
	if (version == 1) {
		ar(CEREAL_NVP(variable), CEREAL_NVP(op));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( UnaryOperation, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(UnaryOperation, "UnaryOperation");

#endif /* SRC_OPERABLES_MATHEMATICS_UNARYOPERATION_H_ */
