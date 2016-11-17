#include "PathSearcherIterator.h"


PathSearcherIterator::PathSearcherIterator(std::shared_ptr<SimplePathSearcher> engine)
{
	this->lastPosition = 0;
	this->engine = engine;
    //this->engine->startSearch();
}

PathSearcherIterator::~PathSearcherIterator()
{
}

int PathSearcherIterator::hasNext()
{
	int has = -1;
	if (lastPosition >= engine->getAvialableFlows()->size()) {
		LOG(DEBUG) << "Calculating next flow...";
		if (engine->calculateNextFlow()) {
			has = engine->getAvialableFlows()->at(lastPosition)->getPaths().size();
		}
	}
	else {
		has = engine->getAvialableFlows()->at(lastPosition)->getPaths().size();
	}
	return has;
}

int PathSearcherIterator::hasNext(std::unordered_set<int> visitados)
{
	int has = -1;
	if (lastPosition >= engine->getAvialableFlows()->size()) {
		LOG(DEBUG) << "Calculating next flow...";
        if (engine->calculateNextFlow(/*visitados, 0*/)) {
			has = engine->getAvialableFlows()->at(lastPosition)->getPaths().size();
		}
	}else {
		has = engine->getAvialableFlows()->at(lastPosition)->getPaths().size();
	}
	return has;
}

ExecutableMachineGraph::FlowPtr PathSearcherIterator::next() throw (std::runtime_error)
{
    ExecutableMachineGraph::FlowPtr next;
	if (lastPosition < engine->getAvialableFlows()->size()) {
		next = engine->getAvialableFlows()->at(lastPosition);
	} else {
		throw(std::runtime_error("iterator is already at the end"));
	}
	lastPosition++;
	return next;
}

ExecutableMachineGraph::FlowPtr PathSearcherIterator::next(std::unordered_set<int> visitados) throw(std::runtime_error)
{
    ExecutableMachineGraph::FlowPtr next;
	if (lastPosition < engine->getAvialableFlows()->size()) {
		next = engine->getAvialableFlows()->at(lastPosition);
	}
	else {
		LOG(DEBUG) << "Calculating next flow...";
        if (engine->calculateNextFlow(/*visitados, 0*/)) {
			next = engine->getAvialableFlows()->at(lastPosition);
		}
		else {
			throw(std::runtime_error("iterator is already at the end"));
		}
	}
	lastPosition++;
	return next;
}

void PathSearcherIterator::begin()
{
	lastPosition = 0;
}

bool PathSearcherIterator::hasEnded()
{
	return (lastPosition >= engine->getAvialableFlows()->size() && engine->hasEnded());
}

int PathSearcherIterator::getIdStart() {
	return engine->getIdStart();
}
