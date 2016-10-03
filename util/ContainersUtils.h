/*
 * ContainersUtils.h
 *
 *  Created on: 12 de may. de 2016
 *      Author: angel
 */

#ifndef UTIL_CONTAINERSUTILS_H_
#define UTIL_CONTAINERSUTILS_H_

#include <vector>
#include <unordered_set>
#include <algorithm>

// boost library
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include "fluidControl/machineGraph/ContainerNode.h"
#include "fluidControl/machineGraph/ContainerNodeType.h"
#include "graph/Node.h"
#include "graph/Graph.h"
#include "graph/Edge.h"

#include "evocodercore_global.h"

class CONTAINERSUTILS_EXPORT ContainersUtils {

public:
	/**
	 * Checks if v1 is compatible with v2, that is , all v1 node types are present in v2.
	 * @param v1 vector to be checked
	 * @param v2 vector to be checked against
	 * @return true if v1 is compatible with v2, false otherwise
	 */
	/*template<class NodeType1,class NodeType2> static bool areSubgraphCompatible(
			const Graph<NodeType1,Edge>::NodeVector & v1,const Graph<NodeType2, Edge>::NodeVector & v2,
			const std::tr1::unordered_set<int> & v2UsedNodes);*/


	template <class NodeType1> static bool isNodeInVector(NodeType1* node, const vector<int> & nodesIds);
	template <class NodeType1, class NodeType2> static bool isNodeInVector(NodeType1* node, const vector<NodeType2*> & nodes);
	template <class NodeType1> static bool isNodeInVector(int node, const vector<NodeType1*> & nodes);

	virtual ~ContainersUtils(){}
private:
	ContainersUtils(){}
	ContainersUtils(const ContainersUtils& c){}
};

template<class NodeType1>
inline bool ContainersUtils::isNodeInVector(NodeType1* node,
		const vector<int>& nodesIds) {

	BOOST_STATIC_ASSERT((boost::is_base_of<Node, NodeType1>::value));

	bool finded = false;
		auto it = nodesIds.begin();
		while(!finded && it != nodesIds.end()) {
			int actual = *it;
			finded  = (node->getContainerId() == actual);
			++it;
		}
		return finded;
}

template<class NodeType1, class NodeType2>
inline bool ContainersUtils::isNodeInVector(NodeType1* node,
		const vector<NodeType2*>& nodes) {

	BOOST_STATIC_ASSERT((boost::is_base_of<Node, NodeType1>::value));
	BOOST_STATIC_ASSERT((boost::is_base_of<Node, NodeType2>::value));

	bool finded = false;
		auto it = nodes.begin();
		while (!finded && it != nodes.end()) {
			Node* actual = *it;
			finded = (node->equals(*actual));
			++it;
		}
		return finded;
}

template<class NodeType>
inline bool ContainersUtils::isNodeInVector(int node,
		const vector<NodeType*>& nodes) {
	BOOST_STATIC_ASSERT((boost::is_base_of<Node, NodeType>::value));

	bool finded = false;
		auto it = nodes.begin();
		while (!finded && it != nodes.end()) {
			Node* actual = *it;
			finded = (actual->getContainerId() == node);
			++it;
		}
		return finded;
}

//template<class NodeType1, class NodeType2>
//inline bool ContainersUtils::areSubgraphCompatible(
//		const Graph<NodeType1, Edge>::NodeVector & v1, const Graph<NodeType2, Edge>::NodeVector & v2,
//		const std::tr1::unordered_set<int>& v2UsedNodes) {
//	// Restrict this template for using only types derived from Node and edge Interfaces
//	BOOST_STATIC_ASSERT((boost::is_base_of<ContainerNode, NodeType1>::value));
//	BOOST_STATIC_ASSERT((boost::is_base_of<ContainerNode, NodeType2>::value));
//
//	bool compatble = true;
//	if (v1.empty() || v2.empty() || v1.size() > v2.size()) {
//		compatble = false;
//	}
//	std::vector<ContainerNodeType> typesChecked;
//	for (auto it1 = v1.begin(); compatble && it1 != v1.end(); ++it1) {
//		ContainerNode* actual1 = (*it1).get();
//		ContainerNodeType actualType1 = *(actual1->getType());
//		if ((v2UsedNodes.find(actual1->getContainerId()) == v2UsedNodes.end())
//				&& find(typesChecked.begin(), typesChecked.end(), actualType1)
//						== typesChecked.end()) {
//			bool encontrado = false;
//			for (auto it2 = v2.begin(); !encontrado && it2 != v2.end(); ++it2) {
//				ContainerNode* actual2 = (*it2).get();
//				ContainerNodeType actualType2 = *(actual2->getType());
//				encontrado = actualType2.isCompatible(actualType1);
//			}
//			compatble = encontrado;
//			typesChecked.push_back(actualType1);
//		}
//	}
//	return compatble;
//}

#endif /* UTIL_CONTAINERSUTILS_H_ */
