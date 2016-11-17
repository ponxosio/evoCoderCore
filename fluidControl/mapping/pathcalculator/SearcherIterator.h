#pragma once

#pragma warning( disable : 4290 )

#include <memory>
#include <unordered_set>
#include <stdexcept>

#include <easylogging++.h>

#include "SearcherInterface.h"

#include "evocodercore_global.h"

class SEARCHERITERATOR_EXPORT SearcherIterator
{
public:
	SearcherIterator(std::shared_ptr<SearcherInterface> engine);
	virtual ~SearcherIterator();

	bool hasNext();

    ExecutableMachineGraph::FlowPtr next() throw (std::runtime_error);

	void begin();
	bool hasEnded();
protected:
	int lastPosition;
	std::shared_ptr<SearcherInterface> engine;
};

