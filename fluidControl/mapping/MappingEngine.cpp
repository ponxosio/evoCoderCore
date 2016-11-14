/*
 * MappingEngine.cpp
 *
 *  Created on: 12 de may. de 2016
 *      Author: angel
 */

#include "MappingEngine.h"

MappingEngine::MappingEngine(MachineGraph* sketch,
	std::shared_ptr<ExecutableMachineGraph> machine) : 
	manager(machine) 
{
	this->sketch = sketch;
	this->machine = machine;

	this->containersMap = new std::unordered_map<int,int>();
	this->numberSolutionsMap = new std::unordered_map<int,int>();
	this->edgeFlowMap = new std::unordered_map<std::pair<int,int>, Flow<Edge>*, PairIntIntHashFunction>();
}

MappingEngine::~MappingEngine() {
	delete containersMap;

	for (auto it = edgeFlowMap->begin(); it != edgeFlowMap->end(); ++it) {
		delete it->second;
	}
	delete edgeFlowMap;
	delete numberSolutionsMap;
}

/*bool MappingEngine::startMapping() {
	bool success = false;
	std::vector<SubGraphSketch> skecthSubgraphs = *sketch->getSubgraphs();
	std::vector<SubGraphMachine>* machineSubgraphs = machine->getSubgraphs();

	if (!skecthSubgraphs.empty() && !machineSubgraphs->empty()) {
		success = trySubgraph(skecthSubgraphs, machineSubgraphs);
	}
	return success;
}*/

bool MappingEngine::startMapping() {
	MachineGraph::ContainerEdgeVector sketchEdgeList(*(sketch->getGraph()->getEdgeList()));
	ExecutableMachineGraph::ExecutableContainerNodeVectorPtr machineSubgraphs = machine->getGraph()->getAllNodes();

	return mapSubgraph(sketchEdgeList, machineSubgraphs);
}

bool MappingEngine::mapSubgraph(MachineGraph::ContainerEdgeVector& edges, ExecutableMachineGraph::ExecutableContainerNodeVectorPtr machineNodes) {
	bool success = false;
	if (!edges.empty()) {
		MachineGraph::ContainerEdgePtr actual = edges.back();
		edges.pop_back();

		shared_ptr<SearcherIterator> it = getAvailableFlows(actual);
		while (!success && it->hasNext()) {
			shared_ptr<Flow<Edge>> actualFlow = it->next();
			if (isAvailable(actualFlow, actual)) {
				addSolution(actual, *(actualFlow.get()));
				success = mapSubgraph(edges, machineNodes);
				if (!success) {
					removeSolution(actual);
				}
			}
		}
		if (!success) {
			edges.push_back(actual);
		}
	} else {
		success = true;
	}
	return success;
}

/*bool MappingEngine::mapSubgraph(MachineGraph::ContainerEdgeVector& edges, ExecutableMachineGraph::ExecutableContainerNodeVectorPtr machineNodes) {
    bool success = false;
    if (!edges.empty()) {
        MachineGraph::ContainerEdgePtr actual = edges.back();
        edges.pop_back();

        FlowsHeap heap = getAvailableFlows(actual);
        while(!success && !heap.empty()) {
            const Flow<Edge> actualFlow = heap.top();
            heap.pop();
            if (isAvailable(actualFlow, actual)) {
                addSolution(actual, actualFlow);
                success = mapSubgraph(edges, machineNodes);
                if (!success) {
                    removeSolution(actual);
                }
            }
        }
        if (!success) {
            LOG(DEBUG) << "no solution for " << actual->toText();
            edges.push_back(actual);
        }
    } else {
        success = true;
    }
    return success;
}*/

void MappingEngine::addSolution(MachineGraph::ContainerEdgePtr edge, const Flow<Edge> & flow) throw(std::invalid_argument){
	Flow<Edge>* ptrFlow = new Flow<Edge>(flow);
	edgeFlowMap->insert(make_pair(std::pair<int,int>(edge->getIdSource(), edge->getIdTarget()), ptrFlow));

    LOG(DEBUG) << "mapped: sketch: " << edge->toText() << ", machine: " << ptrFlow->toText();

	if (containersMap->find(edge->getIdSource()) == containersMap->end()) {
		containersMap->insert(make_pair(edge->getIdSource(), flow.getIdStart()));
		numberSolutionsMap->insert(make_pair(edge->getIdSource(), 1));
	} else {
		if (containersMap->find(edge->getIdSource())->second != flow.getIdStart()) {
			throw(std::invalid_argument("attending to map " +
					patch::to_string(edge->getIdSource()) +
					" with tow different values" +
					patch::to_string(flow.getIdStart()) + "," +
					patch::to_string(containersMap->find(edge->getIdSource())->second)));
		} else {
			numberSolutionsMap->find(edge->getIdSource())->second += 1;
		}
	}

	if (containersMap->find(edge->getIdTarget()) == containersMap->end()) {
			containersMap->insert(make_pair(edge->getIdTarget(), flow.getIdFinish()));
			numberSolutionsMap->insert(make_pair(edge->getIdTarget(), 1));
		} else {
			if (containersMap->find(edge->getIdTarget())->second != flow.getIdFinish()) {
				throw(std::invalid_argument("attending to map " +
						patch::to_string(edge->getIdTarget()) +
						" with tow different values" +
						patch::to_string(flow.getIdFinish()) + "," +
						patch::to_string(containersMap->find(edge->getIdTarget())->second)));
			} else {
				numberSolutionsMap->find(edge->getIdTarget())->second += 1;
			}
		}

	setNodesUsed(flow);
}

void MappingEngine::removeSolution(MachineGraph::ContainerEdgePtr edge) {

    LOG(DEBUG) << " removing solution: " << edge->toText();

	if (numberSolutionsMap->find(edge->getIdSource())->second == 1) {
		containersMap->erase(edge->getIdSource());
		numberSolutionsMap->erase(edge->getIdSource());
	} else {
		numberSolutionsMap->find(edge->getIdSource())->second-=1;
	}

	if (numberSolutionsMap->find(edge->getIdTarget())->second == 1) {
		containersMap->erase(edge->getIdTarget());
		numberSolutionsMap->erase(edge->getIdTarget());
	} else {
		numberSolutionsMap->find(edge->getIdTarget())->second-=1;
	}

	std::pair<int,int> flowsKey(edge->getIdSource(), edge->getIdTarget());
	Flow<Edge>* flow = edgeFlowMap->find(flowsKey)->second;

	unsetNodesUsed(*flow);

	delete flow;
	edgeFlowMap->erase(flowsKey);
}

//bool MappingEngine::trySubgraph(Graph<ContainerNode, Edge>::SubGraph & sketchSubgraphs,
//	Graph<ExecutableContainerNode, Edge>::SubGraphPtr machineSubgraphs) {
//
//	bool success = false;
//	if (!sketchSubgraphs.empty()) {
//		SubGraphSketch actualSketch = sketchSubgraphs.back();
//		sketchSubgraphs.pop_back();
//		for (auto it = machineSubgraphs->begin(); !success && it != machineSubgraphs->end(); ++it) {
//			SubGraphMachine actualMachine = *it;
//			MachineGraph::ContainerNodeVectorPtr nodesSketch = actualSketch.first;
//			ExecutableMachineGraph::ExecutableContainerNodeVectorPtr nodesmachine = actualMachine.first;
//			std::tr1::unordered_set<int>* usedNodes = machine->getUsedNodes();
//			if (ContainersUtils::areSubgraphCompatible<ContainerNode,
//					ExecutableContainerNode>(*nodesSketch, *nodesmachine,
//					*usedNodes)) {
//				std::vector<Edge*> edgesSketch = *actualSketch.second;
//				if (mapSubgraph(edgesSketch,actualMachine.first)) {
//					success = trySubgraph(sketchSubgraphs, machineSubgraphs);
//				}
//			}
//		}
//	} else {
//		success = true;
//	}
//	return success;
//}

Flow<Edge>* MappingEngine::getMappedEdge(MachineGraph::ContainerEdgePtr skectchEdge) throw(std::invalid_argument) {
	auto it = edgeFlowMap->find(std::pair<int,int>(skectchEdge->getIdSource(), skectchEdge->getIdTarget()));
	if (it == edgeFlowMap->end()) {
		throw(std::invalid_argument(
				"the edge (" + patch::to_string(skectchEdge->getIdSource())
						+ "," + patch::to_string(skectchEdge->getIdTarget())
						+ ") has no mapped value"));
	}
	return it->second;
}

int MappingEngine::getMappedContainerId(int sketchContainerId) throw(std::invalid_argument) {
	auto it = containersMap->find(sketchContainerId);
	if (it == containersMap->end()) {
		throw(std::invalid_argument("the container " + patch::to_string(sketchContainerId) + " has no mapped value"));
	}
	return it->second;
}

void MappingEngine::setNodesUsed(const Flow<Edge>& flow) {
	Flow<Edge>::FlowEdgeVector edges = flow.getPaths();
	for(auto it = edges.begin(); it != edges.end(); ++it) {
        machine->addUsedEdge((*it)->getIdSource(), (*it)->getIdTarget());
        machine->addUsedNode((*it)->getIdSource());
		machine->addUsedNode((*it)->getIdTarget());
	}
}

void MappingEngine::unsetNodesUsed(const Flow<Edge>& flow) {
	Flow<Edge>::FlowEdgeVector edges = flow.getPaths();
	for (auto it = edges.begin(); it != edges.end(); ++it) {
        machine->removeUsedEdge((*it)->getIdSource(), (*it)->getIdTarget());
        machine->removeUsedNode((*it)->getIdSource());
		machine->removeUsedNode((*it)->getIdTarget());
	}
}

std::shared_ptr<SearcherIterator> MappingEngine::getAvailableFlows(ExecutableMachineGraph::ExecutableContainerEdgePtr actual) {

	if (!isMapped(actual->getIdSource()) && !isMapped(actual->getIdTarget())) {
		shared_ptr<ContainerNodeType> typeSource = sketch->getContainer(actual->getIdSource())->getType();
		shared_ptr<ContainerNodeType> typeTarget = sketch->getContainer(actual->getIdTarget())->getType();
		return manager.getFlows(typeSource, typeTarget);
	} else if (isMapped(actual->getIdSource())
			&& !isMapped(actual->getIdTarget())) {
		shared_ptr<ContainerNodeType> typeTarget = sketch->getContainer(actual->getIdTarget())->getType();
		int idSourceMachine = containersMap->find(actual->getIdSource())->second;
		return manager.getFlows(idSourceMachine, typeTarget);
	} else if (!isMapped(actual->getIdSource())
			&& isMapped(actual->getIdTarget())) {
		shared_ptr<ContainerNodeType> typeSource = sketch->getContainer(actual->getIdSource())->getType();
		int idTargetMachine = containersMap->find(actual->getIdTarget())->second;
		return manager.getFlows(typeSource, idTargetMachine);
	} else { //both mapped
		int idSourceMachine = containersMap->find(actual->getIdSource())->second;
		int idTargetMachine = containersMap->find(actual->getIdTarget())->second;
		return manager.getFlows(idSourceMachine, idTargetMachine);
	}
}

/*FlowsHeap MappingEngine::getAvailableFlows(ExecutableMachineGraph::ExecutableContainerEdgePtr actual) {

    if (!isMapped(actual->getIdSource()) && !isMapped(actual->getIdTarget())) {
        shared_ptr<ContainerNodeType> typeSource = sketch->getContainer(actual->getIdSource())->getType();
        shared_ptr<ContainerNodeType> typeTarget = sketch->getContainer(actual->getIdTarget())->getType();
        return machine->getAvailableFlows(*typeSource.get(), *typeTarget.get(), *(machine->getGraph()->getAllNodes().get()));
    } else if (isMapped(actual->getIdSource())
            && !isMapped(actual->getIdTarget())) {
        shared_ptr<ContainerNodeType> typeTarget = sketch->getContainer(actual->getIdTarget())->getType();
        int idSourceMachine = containersMap->find(actual->getIdSource())->second;
        return machine->getAvailableFlows(idSourceMachine, *typeTarget.get());
    } else if (!isMapped(actual->getIdSource())
            && isMapped(actual->getIdTarget())) {
        shared_ptr<ContainerNodeType> typeSource = sketch->getContainer(actual->getIdSource())->getType();
        int idTargetMachine = containersMap->find(actual->getIdTarget())->second;
        return machine->getAvailableFlows(*typeSource.get(), idTargetMachine);
    } else { //both mapped
        int idSourceMachine = containersMap->find(actual->getIdSource())->second;
        int idTargetMachine = containersMap->find(actual->getIdTarget())->second;
        return machine->getAvailableFlows(idSourceMachine, idTargetMachine);
    }
}*/

bool MappingEngine::isAvailable(std::shared_ptr<Flow<Edge>> actualFlow, MachineGraph::ContainerEdgePtr actualEdge)
{
	bool available = true;
	int idStart = actualFlow->getIdStart();
	int idFinish = actualFlow->getIdFinish();
	Flow<Edge>::FlowEdgeVector paths = actualFlow->getPaths();

	if (!isMapped(actualEdge->getIdSource())) {
        available = available && machine->isNodeAvailable(idStart);
	}
	if (!isMapped(actualEdge->getIdTarget())) {
        available = available && machine->isNodeAvailable(idFinish);
	}

	for (auto it = paths.begin(); available && it != paths.end(); ++it) {
        available = available && machine->isEdgeAvailable(*it);

		if ((it + 1) != paths.end()) {
            available = available && machine->isNodeAvailable((*it)->getIdTarget());
		} 
	}
	
	return available;
}

void MappingEngine::cleanUsedResources() {
    machine->clearUsedResources();
}

/*bool MappingEngine::isAvailable(const Flow<Edge> actualFlow, MachineGraph::ContainerEdgePtr actualEdge)
{
    bool available = true;
    int idStart = actualFlow.getIdStart();
    int idFinish = actualFlow.getIdFinish();
    Flow<Edge>::FlowEdgeVector paths = actualFlow.getPaths();

    if (!isMapped(actualEdge->getIdSource())) {
        available = machine->isNodeAvailable(idStart);
    }
    if (!isMapped(actualEdge->getIdTarget())) {
        available = machine->isNodeAvailable(idFinish);
    }

    for (auto it = paths.begin(); available && it != paths.end(); ++it) {
        available = machine->isEdgeAvailable(*it);

        if ((it + 1) != paths.end()) {
            available = machine->isNodeAvailable((*it)->getIdTarget());
        }
    }

    return available;
}*/
