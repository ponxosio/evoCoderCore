/*
 * AritmeticOperation.h
 *
 *  Created on: 21 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_OPERABLES_MATHEMATICS_ARITHMETICOPERATION_H_
#define SRC_OPERABLES_MATHEMATICS_ARITHMETICOPERATION_H_

//operators strings
#define PLUS_STRING "+"
#define MINUS_STRING "-"
#define MULTIPLY_STRING "*"
#define DIVIDE_STRING "/"


#include <functional>

//boost
#include <boost/function.hpp>
#include <memory>

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>

//local
#include "MathematicOperable.h"
#include "../../util/Utils.h"

namespace arithmetic {
/*** Enum for the type of arithmetic operator ***/
enum ArithmeticOperator {
	plus, // +
	minus, // -
	multiply, // *
	divide, // /
};
}

/**
 * Class that represents an arithmetic operation between two MathematicVariable s, operator available are + - * \
 */
class ArithmeticOperation: public MathematicOperable {
public:

	ArithmeticOperation();
	ArithmeticOperation(std::shared_ptr<MathematicOperable> left,
			arithmetic::ArithmeticOperator op,
			std::shared_ptr<MathematicOperable> right);

	virtual ~ArithmeticOperation();

	virtual void updateReference(const std::string & reference);

	/**
	 * Returns the numeric value of the variable, resulting from the mathematical operation
	 * @return the numeric value of the variable
	 */
	virtual double getValue() throw (std::invalid_argument);

	inline virtual bool isPhysical() throw (std::invalid_argument) {return (leftVariable.get()->isPhysical() || rightVariable.get()->isPhysical());}

	/**
	 * Compares two Objects implementing this interface
	 * @param obj other object to be compared to
	 * @return true if both are the same, false otherwise.
	 */
	virtual bool equal(const MathematicOperable* obj) const;

	inline virtual  std::string toString() {
		return leftVariable.get()->toString() + " " + getStringOp() + " " + rightVariable.get()->toString();
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);

protected:
	//METHODS
	/**
	 * Returns a function that implements the comparison op
	 * @param op the comparison operation
	 * @return a function that implements the desired comparison
	 */
	static boost::function<double(double, double)> getFunctionType(
			arithmetic::ArithmeticOperator op);

	//Attributes
	/**
	 * Variable on the left of the operation
	 */
	std::shared_ptr<MathematicOperable> leftVariable;
	/**
	 * Variable on the right of the operation
	 */
	std::shared_ptr<MathematicOperable> rightVariable;
	/**
	 * Operator between variables
	 */
	arithmetic::ArithmeticOperator op;

	std::string getStringOp();
};

template<class Archive>
inline void ArithmeticOperation::serialize(Archive& ar,
		const std::uint32_t version) {
	if (version == 1) {
		ar(CEREAL_NVP(leftVariable), CEREAL_NVP(rightVariable), CEREAL_NVP(op));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( ArithmeticOperation, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(ArithmeticOperation, "ArithmeticOperation");

#endif /* SRC_OPERABLES_MATHEMATICS_ARITHMETICOPERATION_H_ */
