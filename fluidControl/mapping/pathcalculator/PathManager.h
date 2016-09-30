#pragma once

#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <tuple>

#include "../../../util/Utils.h"
#include "../../../util/Patch.h"
#include "../../executable/ExecutableMachineGraph.h"
#include "../../machineGraph/ContainerNodeType.h"

#include "PathSearcher.h"
#include "SimplePathSearcher.h"
#include "PathSearcherIterator.h"

#include "SearcherInterface.h"
#include "SearcherIterator.h"
#include "IdSearcher.h"
#include "TypeSearcher.h"

class PathManager
{
public:
	PathManager(std::shared_ptr<ExecutableMachineGraph> machine);
	virtual ~PathManager();

	std::shared_ptr<PathSearcherIterator> getPathSearcher(int idStart, bool reverse);

	std::shared_ptr<SearcherIterator> getFlows(int idStart, int idFinish) throw (std::invalid_argument);
	std::shared_ptr<SearcherIterator> getFlows(int idStart, std::shared_ptr<ContainerNodeType> destinationType) throw(std::invalid_argument);
	std::shared_ptr<SearcherIterator> getFlows(std::shared_ptr<ContainerNodeType> startType, int idFinish) throw(std::invalid_argument);
	std::shared_ptr<SearcherIterator> getFlows(std::shared_ptr<ContainerNodeType> startType, std::shared_ptr<ContainerNodeType> destinationType) throw(std::invalid_argument);

protected:
	std::shared_ptr<ExecutableMachineGraph> machine;

	//internal
	//paths
	std::unordered_map<int, std::shared_ptr<PathSearcher>> searchersMap;
	std::unordered_map<int, std::shared_ptr<PathSearcher>> searchersMapReverse;
	
	//flows
	std::unordered_map<std::tuple<int, int>, std::shared_ptr<SearcherInterface>, PairIntIntHashFunction> idIdMap;
	std::unordered_map<std::tuple<int, int>, std::shared_ptr<SearcherInterface>, PairIntIntHashFunction> idTypeMap;
	std::unordered_map<std::tuple<int, int>, std::shared_ptr<SearcherInterface>, PairIntIntHashFunction> typeIdMap;
	std::unordered_map<std::tuple<int, int>, std::shared_ptr<SearcherInterface>, PairIntIntHashFunction> typeTypeMap;
};

