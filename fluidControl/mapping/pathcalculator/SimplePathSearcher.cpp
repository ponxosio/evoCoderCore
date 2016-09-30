#include "SimplePathSearcher.h"


SimplePathSearcher::SimplePathSearcher(int idStart, std::shared_ptr<ExecutableMachineGraph> machine, bool reverse)
{
	this->idStart = idStart;
	this->machine = machine;
	this->reverse = reverse;

	this->ended = false;
	this->calculatedFlows = make_shared<std::vector<std::shared_ptr<Flow<Edge>>>>();
}

SimplePathSearcher::~SimplePathSearcher()
{
}

bool SimplePathSearcher::calculateNextFlow()
{
	bool finded = false;
	if (!ended) {
		this->calculatedFlows = make_shared<std::vector<std::shared_ptr<Flow<Edge>>>>(machine->getAllFlows(idStart, reverse));
		std::sort(calculatedFlows->begin(), calculatedFlows->end(), FlowPtrComparator<Edge>());
		ended = true;
		finded = true;
	}
	return finded;
}