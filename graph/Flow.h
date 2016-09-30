/*
 * Flow.h
 *
 *  Created on: 6 de may. de 2016
 *      Author: angel
 */

#ifndef SRC_GRAPH_FLOW_H_
#define SRC_GRAPH_FLOW_H_

#include <vector>

// boost library
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>

//local
#include "../util/Patch.h"
#include "Graph.h"
#include "Node.h"
#include "Edge.h"

template<class EdgeType> class Flow {
	// Restrict this template for using only types derived from edge Interfaces
	BOOST_STATIC_ASSERT((boost::is_base_of<Edge, EdgeType>::value));

public:

	//TYPE DEFS
	//typedef Graph<Node, EdgeType>::NodeTypePtr FlowNodePtr;
	typedef typename Graph<Node, EdgeType>::EdgeTypePtr FlowEdgePtr;

	//typedef vector<FlowNodePtr> FlowNodeVector;
	typedef vector<FlowEdgePtr> FlowEdgeVector;

	//typedef FlowNodeVector* FlowNodeVectorPtr;
	typedef FlowEdgeVector* FlowEdgeVectorPtr;
	//

	Flow(const Flow & flow);
	Flow(int idStar, int idFinish);
	Flow(int idStar, int idFinish, const typename FlowEdgeVector & paths);
	virtual ~Flow();

	void prepend(typename FlowEdgePtr edge);
	void append(typename FlowEdgePtr edge);
	void removePath(typename FlowEdgePtr edge);

	bool checkLoop();
	bool equals(const Flow & flow);

	std::string toText();

	inline int getIdFinish() const {
		return idFinish;
	}

	inline void setIdFinish(int idFinish) {
		this->idFinish = idFinish;
	}

	inline int getIdStart() const {
		return idStart;
	}

	inline void setIdStart(int idStart) {
		this->idStart = idStart;
	}

	inline const typename FlowEdgeVector & getPaths() const {
		return paths;
	}

	inline bool operator == (const Flow<EdgeType>& rhs) {
		bool equal = (this->idStart == rhs.idStart) && (this->idFinish == rhs.idFinish);
		if (equal) {
			for (auto its = this->paths.begin(); equal && its != this->paths.end(); ++its) {
				bool finded = false;
				FlowEdgePtr actuals = *its;
				for (auto itr = rhs.paths.begin(); !finded && its != rhs.paths.end(); ++its) {
					FlowEdgePtr actualr = *itr;
					finded = actuals->equals(*(actualr.get()));
				}
				equal = finded;
			}
		}
	}
protected:
	int idStart;
	int idFinish;
	typename FlowEdgeVector paths;
};

template<class EdgeType>
Flow<EdgeType>::Flow(const Flow& flow) {
	this->idStart = flow.idStart;
	this->idFinish = flow.idFinish;

	for (auto it = flow.paths.begin(); it != flow.paths.end(); ++it) {
		typename Graph<Node, EdgeType>::EdgeTypePtr actual = *it;
		this->paths.push_back(actual);
	}
}

template<class EdgeType>
Flow<EdgeType>::Flow(int idStar, int idFinish) {
	this->idStart = idStar;
	this->idFinish = idFinish;
}

template<class EdgeType>
Flow<EdgeType>::Flow(int idStar, int idFinish,
		const typename FlowEdgeVector & paths) {
	this->idStart = idStar;
	this->idFinish = idFinish;

	for (auto it = paths.begin(); it != paths.end(); ++it) {
		typename Graph<Node, EdgeType>::EdgeTypePtr actual = *it;
		this->paths.push_back(actual);
	}
}

template<class EdgeType>
Flow<EdgeType>::~Flow() {
}
template<class EdgeType>
void Flow<EdgeType>::prepend(typename FlowEdgePtr edge) {
	this->paths.insert(paths.begin(), edge);
}

template<class EdgeType>
void Flow<EdgeType>::append(typename FlowEdgePtr edge) {
	this->paths.push_back(edge);
}

template<class EdgeType>
void Flow<EdgeType>::removePath(typename FlowEdgePtr edge) {
	auto it = this->paths.begin();
	while (it != this->paths.end()) {
		typename FlowEdgePtr actual = *it;
		if (actual->equals(*edge)) {
			this->paths.erase(it);
		} else {
			++it;
		}
	}
}

template<class EdgeType>
bool Flow<EdgeType>::checkLoop() {
	bool loop = false;
	int idStart = paths.front()->getIdSource();
	auto it = paths.begin();

	while (!loop && it != paths.end()) {
		typename FlowEdgePtr actual = *it;
		loop = (actual->getIdTarget() == idStart);
		++it;
	}
	return loop;
}

template<class EdgeType>
bool Flow<EdgeType>::equals(const Flow& flow) {
	bool equals = ((flow.idStart == idStart) && (flow.idFinish == idFinish)
			&& (flow.paths.size() == paths.size()));

	auto itCompare = flow.paths.begin();
	for (auto itMe = paths.begin(); equals && itMe != paths.end(); ++itMe) {
		equals = (*itMe)->equals(**itCompare);
		++itCompare;
	}
	return equals;
}

template<class EdgeType>
std::string Flow<EdgeType>::toText() {
	std::string text = patch::to_string(idStart) + "->"
			+ patch::to_string(idFinish) + ":";
	for (auto it = paths.begin(); it != paths.end(); ++it) {
		typename FlowEdgePtr actual = *it;
		text += actual->toText();
	}
	return text;
}

#endif /* SRC_GRAPH_FLOW_H_ */
