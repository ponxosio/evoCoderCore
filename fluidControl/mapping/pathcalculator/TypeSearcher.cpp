#include "TypeSearcher.h"
#include "PathManager.h"
#include "PathSearcherIterator.h"

TypeSearcher::TypeSearcher( const ExecutableMachineGraph::NodeVector & pendingNodes,
	PathManager * manager,
	std::shared_ptr<ContainerNodeType> detinationType,
	std::shared_ptr<ExecutableMachineGraph> machine,
	bool reverse)
{
	this->idInicio = idInicio;
	this->manager = manager;
	this->destinationType = detinationType;
	this->machine = machine;
	this->reverse = reverse;

	this->ended = false;
    this->calculatedFlows = std::make_shared <ExecutableMachineGraph::FlowVector>();

	for (auto it = pendingNodes.begin(); it != pendingNodes.end(); ++it) {
		this->pendingNodes.push_back(*it);
	}

	this->it = std::shared_ptr<PathSearcherIterator>();
}

TypeSearcher::~TypeSearcher()
{
}

bool TypeSearcher::calculateNextFlow()
{
	bool hasNext = false;
	if (!ended) {
		if (it) {
			if (it->hasNext() != -1) {
                ExecutableMachineGraph::FlowPtr nextFlow = it->next();

				int nodeId = 0;
				if (!reverse) {
					nodeId = nextFlow->getIdFinish();
				}
				else {
					nodeId = nextFlow->getIdStart();
				}

				ExecutableMachineGraph::NodePtr endNode = machine->getContainer(nodeId);
				if (endNode->getType()->isCompatible(*(destinationType.get()))) {
					calculatedFlows->push_back(nextFlow);
					hasNext = true;
				}
				else {
					hasNext = calculateNextFlow();
				}
			} else {
				it = std::shared_ptr<PathSearcherIterator>();
				hasNext = calculateNextFlow();
			}
		}
		else if (!pendingNodes.empty()) {
			int backId = pendingNodes.back()->getContainerId();
			this->idInicio = backId;

			it = manager->getPathSearcher(backId, reverse);
			pendingNodes.pop_back();

			hasNext = calculateNextFlow();
		} else {
			ended = true;
		}
	}
	return hasNext;
}
