/*
 * LogicalComparison.h
 *
 *  Created on: 22 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_OPERABLES_COMPARISON_BOOLEANCOMPARISON_H_
#define SRC_OPERABLES_COMPARISON_BOOLEANCOMPARISON_H_

//string operations
#define CONJUNCTION_STRING "&&"
#define DISJUNCTION_STRING "||"

//boost
#include <boost/function.hpp>
#include <memory>

//local
#include "ComparisonOperable.h"
#include "../../util/Utils.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>

namespace logical{
/*** Enum for the type of boolean operator ***/
enum BooleanOperator {
	conjunction, // and
	disjunction, // or 
};
}

/**
 * Class that represent a boolean operation between simple comparisons, for example: (t > 3) && (t < 5)
 */
class BooleanComparison: public ComparisonOperable {
public:
	BooleanComparison() {
		this->left = std::shared_ptr<ComparisonOperable>();
		this->right = std::shared_ptr<ComparisonOperable>();
		this->op = logical::conjunction;
		this->negation = false;
	}
	BooleanComparison(bool negation,
			std::shared_ptr<ComparisonOperable> left,
			logical::BooleanOperator op,
			std::shared_ptr<ComparisonOperable> right);
	virtual ~BooleanComparison();

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
	inline virtual bool isPhysical() {
		return (left.get()->isPhysical() || right.get()->isPhysical());
	}
	/**
	 * Checks if two ComparisonOperable are the same
	 * @param obj other ComparisonOperable to be compared
	 * @return true if they are the same, false otherwise
	 */
	virtual bool equal(ComparisonOperable* obj) const;
	/**
	 * negates this comparison, not(comparison)
	 */
	inline virtual void negate() {negation = !negation;}

	inline virtual std::string toString() {
		std::string neg = negation ? "!" : "";
		return neg + left.get()->toString() + " " + getStringOp() + " " + right.get()->toString();
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	std::string getStringOp();
	/**
	 * Returns a function that implements the boolean operator op
	 * @param op the boolean operation
	 * @return a function that implements the desired operation
	 */
	boost::function<bool(bool, bool)> getFunctionType(
			logical::BooleanOperator op);

	//ATTRIBUTES
	std::shared_ptr<ComparisonOperable> left;
	std::shared_ptr<ComparisonOperable> right;
	logical::BooleanOperator op;
	bool negation;
};

template<class Archive>
inline void BooleanComparison::serialize(Archive& ar,
		const std::uint32_t version) {
	if (version == 1) {
		ar(CEREAL_NVP(left), CEREAL_NVP(right), CEREAL_NVP(op), CEREAL_NVP(negation));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( BooleanComparison, 1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(BooleanComparison, "BooleanComparison");

#endif /* SRC_OPERABLES_COMPARISON_BOOLEANCOMPARISON_H_ */
