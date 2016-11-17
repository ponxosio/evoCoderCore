#pragma once

#pragma warning( disable : 4290 )

#include <memory>
#include <unordered_set>
#include <stdexcept>

#include <easylogging++.h>

#include "SimplePathSearcher.h"

#include "evocodercore_global.h"

class PATHSEARCHERITERATOR_EXPORT PathSearcherIterator
{
public:
    PathSearcherIterator(std::shared_ptr<SimplePathSearcher> engine);
	virtual ~PathSearcherIterator();

	int hasNext();
	int hasNext(std::unordered_set<int> visitados);

    ExecutableMachineGraph::ExecutableContainerFlowPtr next() throw (std::runtime_error);
    ExecutableMachineGraph::ExecutableContainerFlowPtr next(std::unordered_set<int> visitados) throw (std::runtime_error);
	
	void begin();
	bool hasEnded();
	int getIdStart();

protected:
	int lastPosition;
    std::shared_ptr<SimplePathSearcher> engine;
};

