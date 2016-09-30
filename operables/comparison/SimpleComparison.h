/*
 * SimpleComparison.h
 *
 *  Created on: 22 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_OPERABLES_COMPARISON_SIMPLECOMPARISON_H_
#define SRC_OPERABLES_COMPARISON_SIMPLECOMPARISON_H_

//Operators string
#define LESS_EQUAL_STRING "<="
#define LESS_STRING "<"
#define GREAT_EQUAL_STRING ">="
#define GREAT_STRING ">"
#define EQUAL_STRING "="

//boost
#include <memory>
#include <boost/function.hpp>

//local
#include "../../util/Utils.h"
#include "../mathematics/MathematicOperable.h"
#include "ComparisonOperable.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>

namespace comparison {
/*** Enum for the type of comparison operator ***/
enum ComparisonOperator {
	less_equal, // <=
	less, // <
	greater_equal, //>=
	greater, // >
	equal // ==
};
}
/**
 * Class that represents a simple comparison between two variables, for example "a > 3"
 */
class SimpleComparison: public ComparisonOperable {
public:
	SimpleComparison() {
		this->left = std::shared_ptr<MathematicOperable>();
		this->right = std::shared_ptr<MathematicOperable>();
		this->op = comparison::less_equal;
		this->negation = false;
	}
	SimpleComparison(bool negation,
			std::shared_ptr<MathematicOperable> left,
			comparison::ComparisonOperator op,
			std::shared_ptr<MathematicOperable> right);
	virtual ~SimpleComparison();

	virtual void updateReference(const std::string & reference);
	/**
	 * Check if the comparison is true o false
	 * @return true if the comparison is true, false otherwise
	 */
	virtual bool conditionMet();
	/**
	 * Check if the comparison used physical values
	 * @return true if the comparison uses physical values, false otherwise
	 */
	inline virtual bool isPhysical() {return (left.get()->isPhysical() || right.get()->isPhysical());}
	/**
	 * Checks if two ComparisonOperable are the same
	 * @param obj other ComparisonOperable to be compared
	 * @return true if they are the same, false otherwise
	 */
	virtual bool equal(ComparisonOperable* obj) const;

	/**
	 * negates this comparison, not(comparison)
	 */
	inline virtual void negate() {
		negation = !negation;
	}

	inline virtual std::string toString() {
		std::string neg = negation ? "!" : "";
		return  neg + left.get()->toString() + " " + getStingOp() + " " + right.get()->toString();
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	std::string getStingOp();
	/**
	 * Returns a function that implements the comparison op
	 * @param op the comparison operation
	 * @return a function that implements the desired comparison
	 */
	boost::function<bool(double, double)> getFunctionType(
			comparison::ComparisonOperator op);

	//ATRIBUTES
	std::shared_ptr<MathematicOperable> left;
	std::shared_ptr<MathematicOperable> right;
	comparison::ComparisonOperator op;
	bool negation;
};

template<class Archive>
inline void SimpleComparison::serialize(Archive& ar,
		const std::uint32_t version) {
	if (version == 1) {
		ar(CEREAL_NVP(left), CEREAL_NVP(right), CEREAL_NVP(op), CEREAL_NVP(negation));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( SimpleComparison, 1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(SimpleComparison, "SimpleComparison");
#endif /* SRC_OPERABLES_COMPARISON_SIMPLECOMPARISON_H_ */
