/*
 * ExecutableMachineGraph.h
 *
 *  Created on: 9 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_EXECUTABLE_EXECUTABLEMACHINEGRAPH_H_
#define SRC_FLUIDCONTROL_EXECUTABLE_EXECUTABLEMACHINEGRAPH_H_

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
#include "../../util/Patch.h"
#include "../../util/Utils.h"
#include "../../graph/Graph.h"
#include "../../graph/Edge.h"
#include "../../graph/Flow.h"
#include "../../graph/FlowPtrComparator.h"
#include "../machineGraph/ContainerNodeType.h"
#include "containers/ExecutableContainerNode.h"
#include "containers\actuators\communications\CommandSender.h"
#include "containers\actuators\communications\FileSender.h"
#include "containers\actuators\communications\SerialSender.h"

//cereal
#include <cereal\cereal.hpp>
#include <cereal\types\memory.hpp>
#include <cereal\types\unordered_set.hpp>
#include <cereal\types\vector.hpp>
#include <cereal/archives/json.hpp>

class ExecutableMachineGraph {
public:
	//TYPE DEFS
	typedef Graph<ExecutableContainerNode, Edge>::NodeTypePtr ExecutableContainerNodePtr;
	typedef Graph<ExecutableContainerNode, Edge>::EdgeTypePtr ExecutableContainerEdgePtr;

	typedef Graph<ExecutableContainerNode, Edge>::NodeVector ExecutableContainerNodeVector;
	typedef Graph<ExecutableContainerNode, Edge>::EdgeVector ExecutableContainerEdgeVector;

	typedef Graph<ExecutableContainerNode, Edge>::NodeVectorPtr ExecutableContainerNodeVectorPtr;
	typedef Graph<ExecutableContainerNode, Edge>::EdgeVectorPtr ExecutableContainerEdgeVectorPtr;

	typedef Graph<ExecutableContainerNode, Edge>::SubGraphPtr ExecutableContainerSubGraphPtr;

	typedef std::priority_queue<Flow<Edge>, vector<Flow<Edge>>, FlowPtrComparator<Edge>> FlowHeap;

	typedef std::shared_ptr<Graph<ExecutableContainerNode, Edge>> ExecutableNodeGraphPtr;

	typedef std::shared_ptr<std::unordered_set<int>> UsedMapPtr;

	typedef std::shared_ptr<std::unordered_set<std::tuple<int, int>, PairIntIntHashFunction>> UsedEdgeMapPtr;
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
	void addContainer(ExecutableContainerNodePtr node);
	ExecutableContainerNodePtr getContainer(int idConatiner) throw(std::invalid_argument);
	bool connectExecutableContainer(int idSource, int idTarget);
	void printMachine(const std::string & path);

	void updateControlActuators();

    void clearMachine();
	//

	//mapping's operations
	FlowHeap getAvailableFlows(
		const ContainerNodeType & tipoIni,
		const ContainerNodeType & tipofin,
		const ExecutableContainerNodeVector & subgraph);
	FlowHeap getAvailableFlows(int idConatinerInit, const ContainerNodeType & tipofin);
	FlowHeap getAvailableFlows(const ContainerNodeType & tipoIni, int idContainerFin);
	FlowHeap getAvailableFlows(int idInit, int idFin);

	std::vector<std::shared_ptr<Flow<Edge>>> getAllFlows(int idContainer, bool reverse);
	void getAllFlows_recursive(int idStart, 
		ExecutableContainerNodePtr actual, 
		unordered_set<int> visited, 
		vector<std::shared_ptr<Flow<Edge>>> & flows,
		vector<shared_ptr<Edge>> paths, 
		bool reverse);

	void addUsedNode(int nodeId);
	void removeUsedNode(int nodeId);
	void addUsedEdge(int idSorce, int idTarget);
	void removeUsedEdge(int idSorce, int idTarget);

	ExecutableContainerNodeVector getAllCompatibleNodes(const ContainerNodeType & type, const ExecutableContainerNodeVector & nodeList);
	ExecutableContainerEdgeVector getAvailableEdges(ExecutableContainerNodePtr actual, bool reversed);

	bool isNodeAvailable(ExecutableContainerNodePtr node);
	bool isNodeAvailable(int nodeId);

	bool isEdgeAvailable(ExecutableContainerEdgePtr edge);
	bool isEdgeAvailable(int idSource, int idTarget);
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
	inline ExecutableNodeGraphPtr getGraph() {
		return graph;
	}
	inline typename ExecutableContainerSubGraphPtr getSubgraphs() {
		return graph->getSubGraphs();
	}
	inline UsedMapPtr getUsedNodes() {
		return usedNodes;
	}
	inline std::string getName() {
		return name;
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
	ExecutableNodeGraphPtr graph;
	UsedMapPtr usedNodes;
	UsedEdgeMapPtr usedEges;

	//comunications
	unique_ptr<CommandSender> execComInterface;
	unique_ptr<CommandSender> testComInterface;

	//methods
	void getAvailableFlows_recursive_type(int idSource, vector<int> & visitados,
		ExecutableContainerEdgeVector & recorridos,
		FlowHeap & flows,
		ExecutableContainerNodePtr actual,
		const ContainerNodeType & destinationType, bool reverse);
	void getAvailableFlows_recursive_id(int idSource, vector<int> & visitados,
		ExecutableContainerEdgeVector & recorridos,
		FlowHeap & flows,
		ExecutableContainerNodePtr actual, int idDestination);

	ExecutableContainerEdgePtr makeEdge(int idSource, int idTarget);
};

template<class Archive>
inline void ExecutableMachineGraph::serialize(Archive& ar,
	const std::uint32_t version) {
	if (version <= 1) {
		ar(CEREAL_NVP(name), CEREAL_NVP(graph), CEREAL_NVP(usedNodes), CEREAL_NVP(usedEges), CEREAL_NVP(execComInterface), CEREAL_NVP(testComInterface));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION(ExecutableMachineGraph, (int)1);

#endif /* SRC_FLUIDCONTROL_EXECUTABLE_EXECUTABLEMACHINEGRAPH_H_ */
