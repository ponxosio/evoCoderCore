#pragma once

#pragma warning( disable : 4290 )

#include <vector>
#include <memory>
#include <tuple>
#include <stdexcept>
#include <algorithm>

#include "fluidControl/executable/ExecutableMachineGraph.h"
#include "SearcherInterface.h"
#include "graph/FlowPtrComparator.h"

#include "evocodercore_global.h"

class SIMPLEPATHSEARCHER_EXPORT SimplePathSearcher : public SearcherInterface
{
public:
	SimplePathSearcher(int idStart, std::shared_ptr<ExecutableMachineGraph> machine, bool reverse);
	virtual ~SimplePathSearcher();

	bool calculateNextFlow();

    virtual inline std::shared_ptr<ExecutableMachineGraph::FlowVector> getAvialableFlows() {
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
    std::shared_ptr<ExecutableMachineGraph::FlowVector> calculatedFlows;
};

