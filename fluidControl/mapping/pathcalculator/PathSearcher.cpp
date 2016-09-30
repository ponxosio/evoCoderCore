#include "PathSearcher.h"
#include "PathManager.h"
#include "PathSearcherIterator.h"

using namespace std;

PathSearcher::PathSearcher(int idInicio, std::shared_ptr<ExecutableMachineGraph> machine, PathManager* manager, bool reverse) throw (std::invalid_argument)
{
	this->idInicio = idInicio;
	this->machine = machine;
	this->manager = manager;
	this->ended = false;
	this->started = false;
	this->reverse = reverse;
	this->minPath = 1;

	try {
		this->calculatedFlows = make_shared<std::vector<std::shared_ptr<Flow<Edge>>>>();
		this->executionStack = make_shared<std::vector<std::tuple<std::shared_ptr<Edge>, std::shared_ptr<PathSearcherIterator>>>>();

		this->visited.insert(idInicio);
	}
	catch (std::invalid_argument & e) {
		throw(std::invalid_argument("PathSearcher(), exception occured; " + std::string(e.what())));
	}
}

PathSearcher::~PathSearcher()
{
}

bool PathSearcher::calculateNextFlow()
{
	bool finded = false;

	if (!ended) {
		if (executionStack->empty()) {
			ended = true;
		}
		else {
			shared_ptr<PathSearcherIterator> topIt = get<1>(executionStack->back());
			shared_ptr<Edge> edge = get<0>(executionStack->back());
			int idTop = topIt->getIdStart();

			if (ptrStart && 
				topIt == ptrStart) 
			{
				minPath++;
			}

			if (idTop == idInicio)
			{
				if (executionStack->size() <= 2) {
					ptrStart = std::shared_ptr<PathSearcherIterator>();
				}
				if (ptrStart &&
					topIt == ptrStart)
				{
					avancePtrStart();
				}
				executionStack->pop_back();
				finded = calculateNextFlow();
			}
			else {
				int actStackPathSize = topIt->hasNext(visited);
				if (actStackPathSize == -1) {
					if (executionStack->size() <= 2) {
						ptrStart = std::shared_ptr<PathSearcherIterator>();
					}
					if (ptrStart &&
						topIt == ptrStart)
					{
						avancePtrStart();
					}
					executionStack->pop_back();
					finded = calculateNextFlow();
				}
				else if (ptrStart &&
					actStackPathSize > minPath)
				{
					auto last = executionStack->back();
					executionStack->pop_back();
					executionStack->insert(executionStack->begin(), last);

					finded = calculateNextFlow();
				}
				else {
					std::shared_ptr<Flow<Edge>> next = topIt->next(visited);
					Flow<Edge>::FlowEdgeVector path = next->getPaths();

					bool findedVisited = false;
					for (auto it = path.begin(); !findedVisited && it != path.end(); ++it) {
						int idTarget = 0;
						if (!reverse) {
							idTarget = (*it)->getIdTarget();
						}
						else {
							idTarget = (*it)->getIdSource();
						}
						findedVisited = visited.find(idTarget) != visited.end();
					}

					if (!findedVisited) {
						std::shared_ptr<Flow<Edge>> newFlow = make_shared<Flow<Edge>>(*(next.get()));

						if (!reverse) {
							newFlow->setIdStart(idInicio);
							newFlow->prepend(edge);
						}
						else {
							//int idStart = newFlow->getIdStart();
							//newFlow->setIdStart(newFlow->getIdFinish());
							newFlow->setIdFinish(idInicio);
							newFlow->append(edge);
						}

						calculatedFlows->push_back(newFlow);
						finded = true;
					}
					else {
						finded = calculateNextFlow();
					}
				}
			}
		}
	}
	return finded;
}

bool PathSearcher::calculateNextFlow(std::unordered_set<int> externalVisited, int lastStack)
{
	bool finded = false;
	for (auto it = visited.begin(); it != visited.end(); ++it) {
		externalVisited.insert(*it);
	}

	if (!ended) {
		if (executionStack->empty()) {
			ended = true;
		}
		else {
			if (lastStack < executionStack->size()) {
				int idNextStack = (executionStack->size() - 1) - lastStack;
				shared_ptr<PathSearcherIterator> topIt = get<1>(executionStack->at(idNextStack));
				shared_ptr<Edge> edge = get<0>(executionStack->at(idNextStack));
				int idTop = topIt->getIdStart();

				if (ptrStart &&
					topIt == ptrStart)
				{
					minPath++;
				}

				if (idTop == idInicio) {
					if (executionStack->size() <= 2) {
						ptrStart = std::shared_ptr<PathSearcherIterator>();
					}
					if (ptrStart &&
						topIt == ptrStart)
					{
						avancePtrStart();
					}
					executionStack->erase(executionStack->begin() + idNextStack);
					finded = calculateNextFlow(externalVisited, lastStack);

				}
				else if (externalVisited.find(idTop) == externalVisited.end()) {
					int actStackPathSize = topIt->hasNext(externalVisited);
					if (actStackPathSize == -1) {
						if (topIt->hasEnded()) {
							if (executionStack->size() <= 2) {
								ptrStart = std::shared_ptr<PathSearcherIterator>();
							}
							if (ptrStart &&
								topIt == ptrStart)
							{
								avancePtrStart();
							}
							executionStack->erase(executionStack->begin() + idNextStack);
						}
						else {
							lastStack++;
						}
						finded = calculateNextFlow(externalVisited, lastStack);
					}
					else if (ptrStart &&
						(executionStack->size() - lastStack) > 1 &&
						actStackPathSize > minPath)
					{
						auto last = *(executionStack->begin() + idNextStack);
						executionStack->erase(executionStack->begin() + idNextStack);
						executionStack->insert(executionStack->begin(), last);

						finded = calculateNextFlow(externalVisited, lastStack);
					} else {
						std::shared_ptr<Flow<Edge>> next = topIt->next(externalVisited);
						Flow<Edge>::FlowEdgeVector path = next->getPaths();

						bool findedVisited = false;
						for (auto it = path.begin(); !findedVisited && it != path.end(); ++it) {
							int idTarget = 0;
							if (!reverse) {
								idTarget = (*it)->getIdTarget();
							}
							else {
								idTarget = (*it)->getIdSource();
							}
							findedVisited = visited.find(idTarget) != visited.end();
						}

						if (!findedVisited) {
							std::shared_ptr<Flow<Edge>> newFlow = make_shared<Flow<Edge>>(*(next.get()));

							if (!reverse) {
								newFlow->setIdStart(idInicio);
								newFlow->prepend(edge);
							}
							else {
								newFlow->setIdFinish(idInicio);
								newFlow->append(edge);
							}

							calculatedFlows->push_back(newFlow);
						}

						findedVisited = false;
						for (auto it = path.begin(); !findedVisited && it != path.end(); ++it) {
							int idTarget = 0;
							if (!reverse) {
								idTarget = (*it)->getIdTarget();
							}
							else {
								idTarget = (*it)->getIdSource();
							}
							findedVisited = externalVisited.find(idTarget) != externalVisited.end();
						}

						if (!findedVisited) {
							finded = true;
						}
						else {
							finded = calculateNextFlow(externalVisited, lastStack);
						}
					}
				}
				else {
					lastStack++;
					finded = calculateNextFlow(externalVisited, lastStack);
				}
			}
		}
	}
	return finded;
}

void PathSearcher::startSearch()
{
	if (!started) {
		started = true;
		ExecutableMachineGraph::ExecutableContainerNodePtr start = machine->getContainer(idInicio);
		ExecutableMachineGraph::ExecutableContainerEdgeVector pending = machine->getAvailableEdges(start, reverse);

		stackAllEdges(pending);
	}
}

/*bool PathSearcher::popNextEdge()
{
	bool finded = false;
	if (!ended) {
		if (!pending.empty()) {
			shared_ptr<Edge> nextEdge = pending.back();
			pending.pop_back();

			int nextId = 0;
			if (!reverse) {
				nextId = nextEdge->getIdTarget();
			}
			else {
				nextId = nextEdge->getIdSource();
			}
			if (machine->isEdgeAvailable(nextEdge) &&
				machine->isNodeAvailable(nextId) &&
				visited.find(nextId) == visited.end())
			{
				vector<shared_ptr<Edge>> v;
				v.push_back(nextEdge);

				if (!reverse) {
					calculatedFlows->push_back(make_shared<Flow<Edge>>(idInicio, nextId, v));
				}
				else {
					calculatedFlows->push_back(make_shared<Flow<Edge>>(nextId, idInicio, v));
				}

				executionStack->push_back(make_tuple(nextEdge, manager->getPathSearcher(nextId, this->reverse)));
				finded = true;
			}
			else {
				finded = popNextEdge();
			}
		}
	}
	return finded;
}*/

/*bool PathSearcher::popNextEdge(std::unordered_set<int> externalVisited, int lastSeen)
{
	bool finded = false;
	if (!ended) {
		if (!pending.empty()) {
			if (lastSeen < pending.size()) {
				int idnextEdge = (pending.size() - 1) - lastSeen;
				shared_ptr<Edge> nextEdge = pending.at(idnextEdge);
				int nextId = 0;
				if (!reverse) {
					nextId = nextEdge->getIdTarget();
				}
				else {
					nextId = nextEdge->getIdSource();
				}

				if (externalVisited.find(nextId) == externalVisited.end()) {
					pending.erase(pending.begin() + idnextEdge);
					if (machine->isEdgeAvailable(nextEdge) &&
						machine->isNodeAvailable(nextId) &&
						visited.find(nextId) == visited.end())
					{
						vector<shared_ptr<Edge>> v;
						v.push_back(nextEdge);

						if (!reverse) {
							calculatedFlows->push_back(make_shared<Flow<Edge>>(idInicio, nextId, v));
						}
						else {
							calculatedFlows->push_back(make_shared<Flow<Edge>>(nextId, idInicio, v));
						}

						executionStack->push_back(make_tuple(nextEdge, manager->getPathSearcher(nextId, this->reverse)));
						finded = true;
					}
					else {
						finded = popNextEdge(externalVisited, lastSeen);
					}
				}
				else {
					lastSeen++;
					finded = popNextEdge(externalVisited, lastSeen);
				}
			}
		}
	}
	return finded;
}*/

void PathSearcher::stackAllEdges(const ExecutableMachineGraph::ExecutableContainerEdgeVector & pending)
{
	for (auto it = pending.begin(); it != pending.end(); ++it) {
		shared_ptr<Edge> nextEdge = *it;

		int nextId = 0;
		if (!reverse) {
			nextId = nextEdge->getIdTarget();
		}
		else {
			nextId = nextEdge->getIdSource();
		}
		if (/*machine->isEdgeAvailable(nextEdge) &&
			machine->isNodeAvailable(nextId) &&*/
			visited.find(nextId) == visited.end())
		{
			vector<shared_ptr<Edge>> v;
			v.push_back(nextEdge);

			if (!reverse) {
				calculatedFlows->push_back(make_shared<Flow<Edge>>(idInicio, nextId, v));
			}
			else {
				calculatedFlows->push_back(make_shared<Flow<Edge>>(nextId, idInicio, v));
			}

			executionStack->push_back(make_tuple(nextEdge, manager->getPathSearcher(nextId, this->reverse)));
		}
	}

	if (executionStack->size() > 1) {
		ptrStart = get<1>(*(executionStack->begin()));
	}
}

void PathSearcher::avancePtrStart()
{
	bool finded = false;
	for (auto it = executionStack->begin(); it != executionStack->end() && !finded; ++it) {
		std::shared_ptr<PathSearcherIterator> act = get<1>(*it);
		if (act == ptrStart) {
			if ((it + 1) == executionStack->end()) {
				ptrStart = get<1>(*(executionStack->begin() + 1));
			}
			else {
				ptrStart = get<1>(*(it + 1));
			}
			finded = true;
		}
	}
}
