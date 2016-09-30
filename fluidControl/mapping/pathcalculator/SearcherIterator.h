#pragma once

#include <memory>
#include <unordered_set>
#include <stdexcept>

#include <easylogging++.h>

#include "SearcherInterface.h"

class SearcherIterator
{
public:
	SearcherIterator(std::shared_ptr<SearcherInterface> engine);
	virtual ~SearcherIterator();

	bool hasNext();

	std::shared_ptr<Flow<Edge>> next() throw (std::runtime_error);

	void begin();
	bool hasEnded();
protected:
	int lastPosition;
	std::shared_ptr<SearcherInterface> engine;
};

