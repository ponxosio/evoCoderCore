/*
 * ComparationOperable.h
 *
 *  Created on: 22 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_OPERABLES_COMPARISON_COMPARISONOPERABLE_H_
#define SRC_OPERABLES_COMPARISON_COMPARISONOPERABLE_H_

#include <string>

/**
 * Interface that represents all comparation operations
 */
class ComparisonOperable {
public:
	ComparisonOperable(){}
	virtual ~ComparisonOperable(){}

	virtual void updateReference(const std::string & reference) {}
	
	/**
	 * Check if the comparison is true o false
	 * @return true if the comparison is true, false otherwise
	 */
	virtual bool conditionMet() = 0;

	/**
	 * Check if the comparison used physical values
	 * @return true if the comparison uses physical values, false otherwise
	 */
	virtual bool isPhysical() = 0;
	/**
	 * Checks if two ComparisonOperable are the same
	 * @param obj other ComparisonOperable to be compared
	 * @return true if they are the same, false otherwise
	 */
	virtual bool equal(ComparisonOperable* obj) const = 0;
	/**
	 * negates this comparison, not(comparison)
	 */
	virtual void negate() = 0;

	virtual std::string toString() = 0;
};

#endif /* SRC_OPERABLES_COMPARISON_COMPARISONOPERABLE_H_ */
