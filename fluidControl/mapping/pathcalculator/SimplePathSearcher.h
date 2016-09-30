#pragma once

#include <vector>
#include <memory>
#include <tuple>
#include <stdexcept>
#include <algorithm>

#include "../../executable/ExecutableMachineGraph.h"
#include "SearcherInterface.h"
#include "../../../graph/FlowPtrComparator.h"

class SimplePathSearcher : public SearcherInterface
{
public:
	SimplePathSearcher(int idStart, std::shared_ptr<ExecutableMachineGraph> machine, bool reverse);
	virtual ~SimplePathSearcher();

	bool calculateNextFlow();

	virtual inline std::shared_ptr<std::vector<std::shared_ptr<Flow<Edge>>>> getAvialableFlows() {
		return calculatedFlows;
	}

	virtual inline bool hasEnded() {
		return ended;
	}

	virtual inline int getIdStart() {
		return idStart;
	}
protected:
	int idStart;
	bool reverse;
	std::shared_ptr<ExecutableMachineGraph> machine;

	//internal
	bool ended;
	std::shared_ptr<std::vector<std::shared_ptr<Flow<Edge>>>> calculatedFlows;
};

