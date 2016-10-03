#pragma once

#pragma warning( disable : 4290 )

#include <memory>
#include <unordered_set>
#include <stdexcept>

#include <easylogging++.h>

#include "PathSearcher.h"

#include "evocodercore_global.h"

class PATHSEARCHERITERATOR_EXPORT PathSearcherIterator
{
public:
	PathSearcherIterator(std::shared_ptr<PathSearcher> engine);
	virtual ~PathSearcherIterator();

	int hasNext();
	int hasNext(std::unordered_set<int> visitados);

	std::shared_ptr<Flow<Edge>> next() throw (std::runtime_error);
	std::shared_ptr<Flow<Edge>> next(std::unordered_set<int> visitados) throw (std::runtime_error);
	
	void begin();
	bool hasEnded();
	int getIdStart();

protected:
	int lastPosition;
	std::shared_ptr<PathSearcher> engine;
};

