#include "IdSearcher.h"
#include "PathManager.h"
#include "PathSearcherIterator.h"


IdSearcher::IdSearcher(int idInicio, PathManager * manager, int idDestination) throw (std::invalid_argument)
{
	this->idInicio = idInicio;
	this->manager = manager;
	this->idDestination = idDestination;

	this->ended = false;
	this->it = manager->getPathSearcher(idInicio, false);
	this->calculatedFlows = std::make_shared<std::vector<std::shared_ptr<Flow<Edge>>>>();
}

IdSearcher::~IdSearcher()
{
}

bool IdSearcher::calculateNextFlow()
{
	bool hasNext = false;
	if (!ended) {
		if (it->hasNext() != -1) {
			std::shared_ptr<Flow<Edge>> nextFlow = it->next();
			if (nextFlow->getIdFinish() == idDestination) {
				calculatedFlows->push_back(nextFlow);
				hasNext = true;
			} else {
				hasNext = calculateNextFlow();
			}
		} else {
			ended = true;
		}
	}
	return hasNext;
}
