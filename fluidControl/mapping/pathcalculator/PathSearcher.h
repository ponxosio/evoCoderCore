#pragma once

#include <stack>
#include <unordered_set>
#include <vector>
#include <tuple>
#include <stdexcept>

#include "../../executable/ExecutableMachineGraph.h"

class PathSearcherIterator;
class PathManager;

class PathSearcher
{
public:
	PathSearcher(int idInicio, std::shared_ptr<ExecutableMachineGraph> machine, PathManager* manager, bool reverse) throw (std::invalid_argument);
	virtual ~PathSearcher();

	bool calculateNextFlow();
	bool calculateNextFlow(std::unordered_set<int> externalVisited, int LastStack);

	void startSearch();

	inline std::shared_ptr<std::vector<std::shared_ptr<Flow<Edge>>>> getAvialableFlows() {
		return calculatedFlows;
	}

	inline bool hasEnded() {
		return ended;
	}

	inline int getIdStart() {
		return idInicio;
	}

protected:
	int idInicio;
	std::shared_ptr<ExecutableMachineGraph> machine;
	PathManager* manager;
	bool reverse;

	//internal
	std::shared_ptr<PathSearcherIterator> ptrStart;
	int minPath;

	bool started;
	bool ended;
	std::unordered_set<int> visited;
	//ExecutableMachineGraph::ExecutableContainerEdgeVector pending;
	std::shared_ptr<std::vector<std::shared_ptr<Flow<Edge>>>> calculatedFlows;
	std::shared_ptr<std::vector<std::tuple<std::shared_ptr<Edge>, std::shared_ptr<PathSearcherIterator>>>> executionStack;

	//bool popNextEdge();
	//bool popNextEdge(std::unordered_set<int> externalVisited, int lastSeen);

	void stackAllEdges(const ExecutableMachineGraph::ExecutableContainerEdgeVector & pending);
	void avancePtrStart();
};

