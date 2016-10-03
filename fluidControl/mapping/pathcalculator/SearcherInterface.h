#pragma once

#include <memory>
#include <vector>
#include <unordered_set>

#include "../../../graph/Edge.h"
#include "../../../graph/Flow.h"

#include "evocodercore_global.h"

class SEARCHERINTERFACE_EXPORT SearcherInterface
{
public:
	virtual ~SearcherInterface() {}

	virtual bool calculateNextFlow() = 0;
	virtual std::shared_ptr<std::vector<std::shared_ptr<Flow<Edge>>>> getAvialableFlows() = 0;
	virtual bool hasEnded() = 0;

	virtual int getIdStart() = 0;
};

