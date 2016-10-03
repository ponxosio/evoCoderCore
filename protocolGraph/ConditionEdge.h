/*
 * ConditionEdge.h
 *
 *  Created on: 15 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_PROTOCOLGRAPH_CONDITIONEDGE_H_
#define SRC_FLUIDCONTROL_PROTOCOLGRAPH_CONDITIONEDGE_H_

//boost
#include <memory>

//local
#include "../graph/Edge.h"
#include "../util/Utils.h"
#include "../operables/comparison/ComparisonOperable.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>

#include "evocodercore_global.h"

/**
 * Represents an edge in a graph that has a condition, the edge can not be used until the condition is met.
 */
class /*CONDITIONEDGE_EXPORT*/ ConditionEdge: public Edge {
public:

	//Obligatory constructors if is derived from NODE
	ConditionEdge();
	ConditionEdge(const ConditionEdge & edge);
	//
	/**
	 * Makes internal copies of leftVariable and rightVariable
	 * @param comparison comparison operation that must be met in order to use this edge
	 */
	ConditionEdge(int idSource, int idTarget, std::shared_ptr<ComparisonOperable> comparison);
	virtual ~ConditionEdge();

	inline void updateReference(const std::string & reference) 
	{
		comparison->updateReference(reference);
	}

	/**
	 * Checks if the condition to use this edge is met
	 * @return true if the condition is met, false otherwise
	 */
	bool conditionMet();
	/**
	 * Checks if the condition uses physical values
	 * @return true if uses physical variables, false otherwise
	 */
	bool isPhyscal();
	/**
	 * Checks if this edge has certain condition
	 * @param comparison condition to be checked
	 * @return true if this edge has the same condition, false otherwise
	 */
	bool hasCondition(ComparisonOperable* comparison);

	//OVERRIDEN METHODS
	/**
	 * Compares two edges
	 *
	 * @param e other edge to compare
	 * @return true if the edges are equal, false otherwise
	 */
	virtual bool equals(const Edge& e);

	virtual std::string toText();

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	std::shared_ptr<ComparisonOperable> comparison;

};

template<class Archive>
inline void ConditionEdge::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		Edge::serialize(ar, version);
		ar(CEREAL_NVP(comparison));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION(ConditionEdge, (int)1);

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(ConditionEdge, "ConditionEdge");

#endif /* SRC_FLUIDCONTROL_PROTOCOLGRAPH_CONDITIONEDGE_H_ */
