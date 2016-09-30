/*
 * FlowPtrComparator.h
 *
 *  Created on: 6 de may. de 2016
 *      Author: angel
 */

#ifndef SRC_GRAPH_FLOWPTRCOMPARATOR_H_
#define SRC_GRAPH_FLOWPTRCOMPARATOR_H_

#include <memory>
#include "Flow.h"

template<class EdgeType> class FlowPtrComparator {
public:
	FlowPtrComparator() {}
	virtual ~FlowPtrComparator(){}

	inline bool operator()(Flow<EdgeType>* f1, Flow<EdgeType>* f2) const {
		return (f1->getPaths().size() < f2->getPaths().size());
	}

	inline bool operator()(std::shared_ptr<Flow<EdgeType>> f1, std::shared_ptr<Flow<EdgeType>> f2) const {
		return (f1->getPaths().size() < f2->getPaths().size());
	}

	inline bool operator()(Flow<EdgeType> f1, Flow<EdgeType> f2) const {
		return (f1.getPaths().size() < f2.getPaths().size());
	}
};

#endif /* SRC_GRAPH_FLOWPTRCOMPARATOR_H_ */
