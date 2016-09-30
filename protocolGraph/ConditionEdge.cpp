/*
 * ConditionEdge.cpp
 *
 *  Created on: 15 de mar. de 2016
 *      Author: angel
 */

#include "ConditionEdge.h"

ConditionEdge::ConditionEdge() : Edge() {
	this->comparison = std::shared_ptr<ComparisonOperable>();
}

ConditionEdge::ConditionEdge(const ConditionEdge& edge) : Edge(edge) {
	this->comparison = edge.comparison;
}

ConditionEdge::ConditionEdge(int idSource, int idTarget,
		std::shared_ptr<ComparisonOperable> comparison) :
		Edge(idSource, idTarget) {

	this->comparison = comparison;
}

ConditionEdge::~ConditionEdge() {

}

bool ConditionEdge::conditionMet() {
	return comparison.get()->conditionMet();
}

bool ConditionEdge::isPhyscal() {
	return comparison.get()->isPhysical();
}

bool ConditionEdge::hasCondition(ComparisonOperable* comparison) {
	return this->comparison.get()->equal(comparison);
}

bool ConditionEdge::equals(const Edge& e) {
	bool vuelta = false;
	if (Utils::IsType<ConditionEdge, Edge> (&e)) {
		const ConditionEdge* cast = dynamic_cast<const ConditionEdge*>(&e);
		vuelta = ((Edge::equals(e))
				&& (cast->comparison.get()->equal(this->comparison.get())));
	}
	return vuelta;
}

std::string ConditionEdge::toText() {
	return patch::to_string(idSource) + "->" + patch::to_string(idTarget) + "[label = \"" + comparison.get()->toString()  + "\"];";
}
