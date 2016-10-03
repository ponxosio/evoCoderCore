/*
 * ProtocolGraph.h
 *
 *  Created on: 9 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_PROTOCOLGRAPH_PROTOCOLGRAPH_H_
#define SRC_FLUIDCONTROL_PROTOCOLGRAPH_PROTOCOLGRAPH_H_

#include <string>

//data structures
#include <vector>

//lib
#include <easylogging++.h>

//local
#include "graph/Graph.h"
#include "operables/VariableTable.h"
#include "ConditionEdge.h"
#include "OperationNode.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>

#include "evocodercore_global.h"

class PROTOCOLGRAPH_EXPORT ProtocolGraph {
public:

	//TYPE DEFS
	typedef Graph<OperationNode, ConditionEdge>::NodeTypePtr ProtocolNodePtr;
	typedef Graph<OperationNode, ConditionEdge>::EdgeTypePtr ProtocolEdgePtr;

	typedef Graph<OperationNode, ConditionEdge>::NodeVector ProtocolNodeVector;
	typedef Graph<OperationNode, ConditionEdge>::EdgeVector ProtocolEdgeVector;

	typedef Graph<OperationNode, ConditionEdge>::NodeVectorPtr ProtocolNodeVectorPtr;
	typedef Graph<OperationNode, ConditionEdge>::EdgeVectorPtr ProtocolEdgeVectorPtr;
	//

	//static
	static void toJSON(const std::string & path, const ProtocolGraph & machine);
	static ProtocolGraph* fromJSON(const std::string & path);
	//

	ProtocolGraph();
	ProtocolGraph(const ProtocolGraph & prot);
	ProtocolGraph(const std::string & name);
	virtual ~ProtocolGraph();

	bool addOperation(ProtocolNodePtr node);
	bool connectOperation(ProtocolEdgePtr edge);
	bool connectOperation(ProtocolNodePtr nodeSource, ProtocolNodePtr nodeTarget, std::shared_ptr<ComparisonOperable> comparison);

	void updateReference(const std::string & reference);

	ProtocolNodePtr getStart();
	void setStartNode(int idStart);

	inline typename ProtocolNodePtr getNode(int idNode) {
		return graph->getNode(idNode);
	}
	inline const ProtocolEdgeVectorPtr getProjectingEdges(int idNode) {
		return graph->getLeavingEdges(idNode);
	}
	inline typename ProtocolNodeVectorPtr getAllNodes() {
		return graph->getAllNodes();
	}

	inline void printProtocol(const std::string & path) {
		graph->saveGraph(path);
	}

	inline const std::string& getName() const {
		return name;
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	int idStart;
	std::string name;
	std::shared_ptr<Graph<OperationNode, ConditionEdge>> graph;

	ProtocolEdgePtr makeEdge(int idSource, int idTarget, std::shared_ptr<ComparisonOperable> comparison);

};

template<class Archive>
inline void ProtocolGraph::serialize(Archive& ar,
	const std::uint32_t version) {
	if (version <= 1) {
		ar(CEREAL_NVP(idStart), CEREAL_NVP(name), CEREAL_NVP(graph));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION(ProtocolGraph, (int)1);

#endif /* SRC_FLUIDCONTROL_PROTOCOLGRAPH_PROTOCOLGRAPH_H_ */
