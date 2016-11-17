#ifndef FLOWGENERATOR_H
#define FLOWGENERATOR_H

#include <unordered_set>
#include <vector>
#include <stdexcept>

// boost library
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>

//local
#include "graph/Edge.h"
#include "graph/edgehash.h"
#include "graph/Flow.h"
#include "util/Utils.h"

template<class EdgeType> class  FlowGenerator {
    // Restrict this template for using only types derived from edge Interfaces
    BOOST_STATIC_ASSERT((boost::is_base_of<Edge, EdgeType>::value));

public:
    typedef std::unordered_map<std::shared_ptr<EdgeType>, int, EdgeHash<EdgeType>, EdgeHash<EdgeType>> EdgesSet;

    FlowGenerator();
    virtual ~FlowGenerator();

    std::shared_ptr<Flow<EdgeType>> makePossibleFlowsBacktraking() throw(std::runtime_error);
    void addEdge(std::shared_ptr<EdgeType> edge);
    void removeEdge(std::shared_ptr<EdgeType> edge);
    void clearEdges();

protected:
    EdgesSet edgesSet;

    bool makePossibleFlows_recursive(std::vector<std::shared_ptr<EdgeType>> & processedFlow,
                                     std::shared_ptr<EdgeType> lastEdgeProcessed,
                                     const EdgesSet & remaininEdgesSet);
};

template<class EdgeType>
FlowGenerator<EdgeType>::FlowGenerator() {

}

template<class EdgeType>
FlowGenerator<EdgeType>::~FlowGenerator() {

}

template<class EdgeType>
std::shared_ptr<Flow<EdgeType>> FlowGenerator<EdgeType>::makePossibleFlowsBacktraking() throw(std::runtime_error){
    if (!edgesSet.empty()) {
        EdgesSet tempSet(edgesSet);

        bool finded = false;
        std::vector<std::shared_ptr<EdgeType>> calculatedFlow;
        for (auto it = edgesSet.begin(); !finded && (it != edgesSet.end()); ++it) {
            std::shared_ptr<EdgeType> initFlowEdge = it->first;

            //add solution
            auto itTemp = tempSet.find(initFlowEdge);
            tempSet.erase(itTemp);
            calculatedFlow.push_back(initFlowEdge);

            //recursive call
            finded = makePossibleFlows_recursive(calculatedFlow, initFlowEdge, tempSet);

            //remove solution if failure
            if (!finded) {
                tempSet.insert(std::make_pair(initFlowEdge,1));
                calculatedFlow.pop_back();
            }
        }

        if (finded) {
            //construct solution
            int idSource = calculatedFlow.at(0)->getIdSource();
            int idTarget = calculatedFlow.back()->getIdTarget();

            return std::make_shared<Flow<EdgeType>>(idSource, idTarget, calculatedFlow);
        } else {
            std::string edgesSetStr = "";
            for (auto it: edgesSet) {
                std::shared_ptr<Edge> edge = it.first;
                edgesSetStr += edge->toText();
            }
            throw (std::runtime_error("imposible to make an unique flow with all the edges in the generator: " + edgesSetStr));
        }
    } else {
        std::shared_ptr<Flow<EdgeType>> flow;
        return flow;
    }
}

template<class EdgeType>
void FlowGenerator<EdgeType>::addEdge(std::shared_ptr<EdgeType> edge) {
    auto it = edgesSet.find(edge);
    if (it == edgesSet.end()) {
        edgesSet.insert(std::make_pair(edge,1));
    } else {
        int numEdges = it->second;
        it->second = numEdges + 1;
    }
}

template<class EdgeType>
void FlowGenerator<EdgeType>::removeEdge(std::shared_ptr<EdgeType> edge) {
    auto it = edgesSet.find(edge);
    if (it != edgesSet.end()) {
        if (it->second == 1) {
            edgesSet.erase(it);
        } else {
            int numEdges = it->second;
            it->second = numEdges - 1;
        }
    }
}

template<class EdgeType>
void FlowGenerator<EdgeType>::clearEdges() {
    edgesSet.clear();
}

template<class EdgeType>
bool FlowGenerator<EdgeType>::makePossibleFlows_recursive(std::vector<std::shared_ptr<EdgeType>> & processedFlow,
                                 std::shared_ptr<EdgeType> lastEdgeProcessed,
                                 const EdgesSet & remaininEdgesSet)
{
    bool finish = false;
    if(!remaininEdgesSet.empty()) {
        EdgesSet tempSet(remaininEdgesSet);
        for(auto it = remaininEdgesSet.begin(); !finish && it != remaininEdgesSet.end(); ++it) {
            std::shared_ptr<EdgeType> actualFlowEdge = it->first;

            //check compatibility
            if (lastEdgeProcessed->getIdTarget() == actualFlowEdge->getIdSource()) {
                //add solution
                auto itTemp = tempSet.find(actualFlowEdge);
                tempSet.erase(itTemp);
                processedFlow.push_back(actualFlowEdge);

                //recursive call
                finish = makePossibleFlows_recursive(processedFlow, actualFlowEdge, tempSet);

                //remove solution if failure
                if (!finish) {
                    tempSet.insert(std::make_pair(actualFlowEdge, 1));
                    processedFlow.pop_back();
                }
            }
        }
    } else {
        finish = true;
    }
    return finish;
}

#endif // FLOWGENERATOR_H
