#pragma once

#include <vector>
#include <memory>
#include <tuple>
#include <stdexcept>

#include "../../../graph/Edge.h"
#include "../../../graph/Flow.h"
#include "SearcherInterface.h"

class PathSearcherIterator;
class PathManager;

class IdSearcher : public SearcherInterface
{
public:
	IdSearcher(int idInicio, PathManager* manager, int idDestination) throw (std::invalid_argument);
	virtual ~IdSearcher();

	bool calculateNextFlow();

	virtual inline std::shared_ptr<std::vector<std::shared_ptr<Flow<Edge>>>> getAvialableFlows() {
		return calculatedFlows;
	}

	virtual inline bool hasEnded() {
		return ended;
	}

	virtual inline int getIdStart() {
		return idInicio;
	}
protected:
	int idInicio;
	PathManager* manager;
	int idDestination;

	//internal
	bool ended;
	std::shared_ptr<PathSearcherIterator> it;
	std::shared_ptr<std::vector<std::shared_ptr<Flow<Edge>>>> calculatedFlows;
};

