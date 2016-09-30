#include "SearcherIterator.h"



SearcherIterator::SearcherIterator(std::shared_ptr<SearcherInterface> engine)
{
	this->lastPosition = 0;
	this->engine = engine;
}

SearcherIterator::~SearcherIterator()
{
}

bool SearcherIterator::hasNext()
{
	bool has = true;
	if (lastPosition >= engine->getAvialableFlows()->size()) {
		has = engine->calculateNextFlow();
	}
	return has;
}

std::shared_ptr<Flow<Edge>> SearcherIterator::next() throw(std::runtime_error)
{
	std::shared_ptr<Flow<Edge>> next;
	if (lastPosition < engine->getAvialableFlows()->size()) {
		next = engine->getAvialableFlows()->at(lastPosition);
	}
	else {
		throw(std::runtime_error("iterator is already at the end"));
	}
	lastPosition++;
	return next;
}

void SearcherIterator::begin()
{
	this->lastPosition = 0;
}

bool SearcherIterator::hasEnded()
{
	return engine->hasEnded();
}

