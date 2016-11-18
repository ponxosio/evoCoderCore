/*
 * ExecutableMachineGraph.h
 *
 *  Created on: 9 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_EXECUTABLE_EXECUTABLEMACHINEGRAPH_H_
#define SRC_FLUIDCONTROL_EXECUTABLE_EXECUTABLEMACHINEGRAPH_H_

#pragma warning( disable : 4290 )

#include <string>
#include <stdexcept>

 //vector
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <tuple>
#include <memory>

//local
#include "util/Patch.h"
#include "util/Utils.h"
#include "graph/Graph.h"
#include "graph/Edge.h"
#include "graph/Flow.h"
#include "graph/FlowPtrComparator.h"
#include "fluidControl/executable/conditionalflowedge.h"
#include "fluidControl/machineGraph/ContainerNodeType.h"
#include "fluidControl/executable/containers/ExecutableContainerNode.h"
#include "fluidControl/executable/containers/actuators/communications/CommandSender.h"
#include "fluidControl/executable/containers/actuators/communications/FileSender.h"
#include "fluidControl/executable/containers/actuators/communications/SerialSender.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/archives/json.hpp>

#include "evocodercore_global.h"

class EXECUTABLEMACHINEGRAPH_EXPORT ExecutableMachineGraph {
public:
	//TYPE DEFS
    typedef ConditionalFlowEdge EdgeType;
    typedef ExecutableContainerNode NodeType;

    typedef Graph<NodeType, EdgeType>::NodeTypePtr NodePtr;
    typedef Graph<NodeType, EdgeType>::EdgeTypePtr EdgePtr;

    typedef Graph<NodeType, EdgeType>::NodeVector NodeVector;
    typedef Graph<NodeType, EdgeType>::EdgeVector EdgeVector;

    typedef Graph<NodeType, EdgeType>::NodeVectorPtr NodeVectorPtr;
    typedef Graph<NodeType, EdgeType>::EdgeVectorPtr EdgeVectorPtr;

    typedef Graph<NodeType, EdgeType>::SubGraphPtr SubGraphPtr;

    typedef Flow<EdgeType> FlowType;
    typedef std::shared_ptr<Flow<EdgeType>> FlowPtr;
    typedef std::vector<FlowPtr> FlowVector;

    typedef std::priority_queue<FlowType, vector<FlowType>, FlowPtrComparator<EdgeType>> FlowHeap;

    typedef std::shared_ptr<Graph<NodeType, EdgeType>> GraphPtr;

	typedef std::shared_ptr<std::unordered_set<int>> UsedMapPtr;

	typedef std::shared_ptr<std::unordered_set<std::tuple<int, int>, PairIntIntHashFunction>> UsedEdgeMapPtr;

    typedef ConditionalFlowEdge::AllowedEdgeSet AllowedEdgesSet;
	//

	//static
	static void toJSON(const std::string & path, const ExecutableMachineGraph & machine);
	static ExecutableMachineGraph* fromJSON(const std::string & path);
	//
	
	//constructors
	ExecutableMachineGraph();
	ExecutableMachineGraph(const ExecutableMachineGraph & exMachine);
	ExecutableMachineGraph(const std::string & name, std::unique_ptr<CommandSender> execComInterface, std::unique_ptr<CommandSender> testComInterface);
	virtual ~ExecutableMachineGraph();
	//

	//graph's operations
    void addContainer(NodePtr node, const std::string & alias = "");
    std::string getAlias(int id);
    NodePtr getContainer(int idConatiner) throw(std::invalid_argument);
    EdgePtr getEdge(int idSource, int idTarget) throw(std::invalid_argument);
    bool connectExecutableContainer(int idSource, int idTarget, const AllowedEdgesSet & allowedEdges);
	void printMachine(const std::string & path);

	void updateControlActuators();

    void clearMachine();
	//

	//mapping's operations
	FlowHeap getAvailableFlows(
		const ContainerNodeType & tipoIni,
		const ContainerNodeType & tipofin,
        const NodeVector & subgraph);
	FlowHeap getAvailableFlows(int idConatinerInit, const ContainerNodeType & tipofin);
	FlowHeap getAvailableFlows(const ContainerNodeType & tipoIni, int idContainerFin);
	FlowHeap getAvailableFlows(int idInit, int idFin);

    FlowVector getAllFlows(int idContainer, bool reverse);
	void getAllFlows_recursive(int idStart, 
        NodePtr actual,
		unordered_set<int> visited, 
        FlowVector & flows,
        EdgeVector paths,
		bool reverse);

	void addUsedNode(int nodeId);
	void removeUsedNode(int nodeId);
	void addUsedEdge(int idSorce, int idTarget);
	void removeUsedEdge(int idSorce, int idTarget);

    NodeVector getAllCompatibleNodes(const ContainerNodeType & type, const NodeVector & nodeList);
    EdgeVector getAvailableEdges(NodePtr actual, bool reversed);

    bool isNodeAvailable(NodePtr node);
	bool isNodeAvailable(int nodeId);

    bool isEdgeAvailable(EdgePtr edge);
	bool isEdgeAvailable(int idSource, int idTarget);

    void clearUsedResources();
	//

	//inlines
	inline bool existsContainer(int idContainer) {
		return (graph->getNode(idContainer) != NULL);
	}

	inline bool areConected(int idSource, int idTarget) {
		return (graph->areConnected(idSource, idTarget));
	}
	inline void saveGraph(const std::string & path) {
		graph->saveGraph(path);
	}

	//getters & setters
    inline GraphPtr getGraph() {
		return graph;
	}
    inline typename SubGraphPtr getSubgraphs() {
		return graph->getSubGraphs();
	}
	inline UsedMapPtr getUsedNodes() {
		return usedNodes;
	}
	inline std::string getName() {
		return name;
	}

    inline void setTestCommunicationsPrototype(std::unique_ptr<CommandSender> testPrototype) {
        testComInterface = std::move(testPrototype);
    }

    inline void setExecCommunicationsPrototype(std::unique_ptr<CommandSender> execPrototype) {
        execComInterface = std::move(execPrototype);
    }

	//Volume
	float getVolume(int idContainer);
	void addVolume(int idContainer, float volume);
	void substractVolume(int idContainer, float volume);

	//communications
	CommandSender* getTestCommunicationsPrototypeCopy();
	CommandSender* getExecCommunicationsPrototypeCopy();
	void updateCommunicationsInterface(int idCommunication);

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);

protected:
	std::string name;
    std::unordered_map<int, std::string> idAliasMap;
    GraphPtr graph;
	UsedMapPtr usedNodes;
	UsedEdgeMapPtr usedEges;

	//comunications
	unique_ptr<CommandSender> execComInterface;
	unique_ptr<CommandSender> testComInterface;

	//methods
	void getAvailableFlows_recursive_type(int idSource, vector<int> & visitados,
        EdgeVector & recorridos,
		FlowHeap & flows,
        NodePtr actual,
		const ContainerNodeType & destinationType, bool reverse);
	void getAvailableFlows_recursive_id(int idSource, vector<int> & visitados,
        EdgeVector & recorridos,
		FlowHeap & flows,
        NodePtr actual, int idDestination);

    EdgePtr makeEdge(int idSource, int idTarget, const AllowedEdgesSet & allowedEdges);
};

template<class Archive>
inline void ExecutableMachineGraph::serialize(Archive& ar,
	const std::uint32_t version) {
	if (version <= 1) {
        ar(CEREAL_NVP(name),
           CEREAL_NVP(graph),
           CEREAL_NVP(usedNodes),
           CEREAL_NVP(usedEges),
           CEREAL_NVP(execComInterface),
           CEREAL_NVP(testComInterface),
           CEREAL_NVP(idAliasMap));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION(ExecutableMachineGraph, (int)1);

#endif /* SRC_FLUIDCONTROL_EXECUTABLE_EXECUTABLEMACHINEGRAPH_H_ */
