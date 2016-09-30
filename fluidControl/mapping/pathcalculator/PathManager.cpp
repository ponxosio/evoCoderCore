#include "PathManager.h"

using namespace std;

PathManager::PathManager(std::shared_ptr<ExecutableMachineGraph> machine)
{
	this->machine = machine;
}

PathManager::~PathManager()
{
}

std::shared_ptr<PathSearcherIterator> PathManager::getPathSearcher(int idStart, bool reverse) throw (std::invalid_argument)
{
	if (!reverse) {
		auto it = searchersMap.find(idStart);
		if (it != searchersMap.end()) {
			std::shared_ptr<PathSearcher> searcher = it->second;
			return make_shared<PathSearcherIterator>(searcher);
		}
		else {
			try {
				std::shared_ptr<PathSearcher> newSearcher = std::make_shared<PathSearcher>(idStart, machine, this, reverse);
				//std::shared_ptr<SearcherInterface> newSearcher = make_shared<SimplePathSearcher>(idStart, machine, reverse);

				searchersMap.insert(std::make_pair(idStart, newSearcher));

				return make_shared<PathSearcherIterator>(newSearcher);
			} catch (std::invalid_argument & e) {
				throw("PathManager::getPatgSearcher(), exception occured: " + string(e.what()));
			}
		}
	} else {
		auto it = searchersMapReverse.find(idStart);
		if (it != searchersMapReverse.end()) {
			std::shared_ptr<PathSearcher> searcher = it->second;
			return make_shared<PathSearcherIterator>(searcher);
		}
		else {
			try {
				std::shared_ptr<PathSearcher> newSearcher = std::make_shared<PathSearcher>(idStart, machine, this, reverse);
				//std::shared_ptr<SearcherInterface> newSearcher = make_shared<SimplePathSearcher>(idStart, machine, reverse);

				searchersMapReverse.insert(std::make_pair(idStart, newSearcher));

				return make_shared<PathSearcherIterator>(newSearcher);
			}
			catch (std::invalid_argument & e) {
				throw("PathManager::getPatgSearcher(), exception occured: " + string(e.what()));
			}
		}
	}
}

std::shared_ptr<SearcherIterator> PathManager::getFlows(int idStart, int idFinish)
{
	auto it = idIdMap.find(make_tuple(idStart,idFinish));
	if (it != idIdMap.end()) {
		std::shared_ptr<SearcherInterface> searcher = it->second;
		return make_shared<SearcherIterator>(searcher);
	} else {
		try {
			std::shared_ptr<SearcherInterface> newSearcher = std::make_shared<IdSearcher>(idStart, this, idFinish);

			idIdMap.insert(std::make_pair(make_tuple(idStart, idFinish), newSearcher));

			return make_shared<SearcherIterator>(newSearcher);
		} catch (std::invalid_argument & e) {
			throw("PathManager::getPatgFlow(int,int), exception occured: " + string(e.what()));
		}
	}
}

std::shared_ptr<SearcherIterator> PathManager::getFlows(int idStart, std::shared_ptr<ContainerNodeType> destinationType) throw (std::invalid_argument)
{
	int hashCode = destinationType->calculateHashCode();
	auto it = idTypeMap.find(make_tuple(idStart, hashCode));
	if (it != idTypeMap.end()) {
		std::shared_ptr<SearcherInterface> searcher = it->second;
		return make_shared<SearcherIterator>(searcher);
	}
	else {
		try {
			ExecutableMachineGraph::ExecutableContainerNodeVector pending;
			pending.push_back(machine->getContainer(idStart));

			std::shared_ptr<SearcherInterface> newSearcher = std::make_shared<TypeSearcher>(pending, this, destinationType, machine, false);

			idTypeMap.insert(std::make_pair(make_tuple(idStart, hashCode), newSearcher));

			return make_shared<SearcherIterator>(newSearcher);
		} catch (std::invalid_argument & e) {
			throw(std::invalid_argument("PathManager::getFlows(id,ContainerNodeType), exception occured, " + string(e.what())));
		}
	}
}

std::shared_ptr<SearcherIterator> PathManager::getFlows(std::shared_ptr<ContainerNodeType> startType, int idFinish) throw(std::invalid_argument)
{
	int hashCode = startType->calculateHashCode();
	auto it = typeIdMap.find(make_tuple(hashCode, idFinish));
	if (it != typeIdMap.end()) {
		std::shared_ptr<SearcherInterface> searcher = it->second;
		return make_shared<SearcherIterator>(searcher);
	}
	else {
		try {
			ExecutableMachineGraph::ExecutableContainerNodeVector pending;
			pending.push_back(machine->getContainer(idFinish));

			std::shared_ptr<SearcherInterface> newSearcher = std::make_shared<TypeSearcher>(pending, this, startType, machine, true);

			typeIdMap.insert(std::make_pair(make_tuple(hashCode, idFinish), newSearcher));

			return make_shared<SearcherIterator>(newSearcher);
		}
		catch (std::invalid_argument & e) {
			throw(std::invalid_argument("PathManager::getFlows(ContainerNodeType, id), exception occured, " + string(e.what())));
		}
	}
}

std::shared_ptr<SearcherIterator> PathManager::getFlows(std::shared_ptr<ContainerNodeType> startType, std::shared_ptr<ContainerNodeType> destinationType) throw(std::invalid_argument)
{
	int hashCodeS = startType->calculateHashCode();
	int hashCodeD = destinationType->calculateHashCode();
	auto it = typeTypeMap.find(make_tuple(hashCodeS, hashCodeD));
	if (it != typeTypeMap.end()) {
		std::shared_ptr<SearcherInterface> searcher = it->second;
		return make_shared<SearcherIterator>(searcher);
	}
	else {
		try {
			ExecutableMachineGraph::ExecutableContainerNodeVector pending = machine->getAllCompatibleNodes(*(startType.get()), *(machine->getGraph()->getAllNodes().get()));

			std::shared_ptr<SearcherInterface> newSearcher = std::make_shared<TypeSearcher>(pending, this, destinationType, machine, false);

			typeTypeMap.insert(std::make_pair(make_tuple(hashCodeS, hashCodeD), newSearcher));

			return make_shared<SearcherIterator>(newSearcher);
		}
		catch (std::invalid_argument & e) {
			throw(std::invalid_argument("PathManager::getFlows(ContainerNodeType, ContainerNodeType), exception occured, " + string(e.what())));
		}
	}
}

