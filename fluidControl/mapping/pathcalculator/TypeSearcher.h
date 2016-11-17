#pragma once

#pragma warning( disable : 4290 )

#include <vector>
#include <memory>
#include <tuple>
#include <stdexcept>

#include "fluidControl/executable/ExecutableMachineGraph.h"
#include "fluidControl/machineGraph/ContainerNodeType.h"
#include "SearcherInterface.h"

#include "evocodercore_global.h"

class PathSearcherIterator;
class PathManager;

class TYPESEARCHER_EXPORT TypeSearcher : public SearcherInterface
{
public:
	TypeSearcher(const ExecutableMachineGraph::ExecutableContainerNodeVector & pendingNodes,
		PathManager* manager, 
		std::shared_ptr<ContainerNodeType> detinationType,
		std::shared_ptr<ExecutableMachineGraph> machine,
		bool reverse);

	virtual ~TypeSearcher();

	virtual bool calculateNextFlow();

    virtual inline std::shared_ptr<ExecutableMachineGraph::ExecutableContainerFlowVector> getAvialableFlows() {
		return calculatedFlows;
	}

	virtual inline bool hasEnded() {
		return ended;
	}

	virtual inline int getIdStart() {
		return idInicio;
	}

protected:
	int idInicio;
	bool reverse;
	ExecutableMachineGraph::ExecutableContainerNodeVector pendingNodes;
	std::shared_ptr<ContainerNodeType> destinationType;
	std::shared_ptr<ExecutableMachineGraph> machine;
	PathManager* manager;

	//internal
	bool ended;
	std::shared_ptr<PathSearcherIterator> it;
    std::shared_ptr<ExecutableMachineGraph::ExecutableContainerFlowVector> calculatedFlows;
};

