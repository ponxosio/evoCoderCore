#include "SimplePathSearcher.h"


SimplePathSearcher::SimplePathSearcher(int idStart, std::shared_ptr<ExecutableMachineGraph> machine, bool reverse)
{
	this->idStart = idStart;
	this->machine = machine;
	this->reverse = reverse;

	this->ended = false;
    this->calculatedFlows = make_shared<ExecutableMachineGraph::ExecutableContainerFlowVector>();
}

SimplePathSearcher::~SimplePathSearcher()
{
}

bool SimplePathSearcher::calculateNextFlow()
{
	bool finded = false;
	if (!ended) {
        this->calculatedFlows = make_shared<ExecutableMachineGraph::ExecutableContainerFlowVector>(machine->getAllFlows(idStart, reverse));
        std::sort(calculatedFlows->begin(), calculatedFlows->end(), FlowPtrComparator<ExecutableMachineGraph::EdgeType>());
		ended = true;
		finded = true;
	}
	return finded;
}
