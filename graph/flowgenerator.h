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

#include "evocodercore_global.h"

template<class EdgeType> class  FLOWGENERATOR_EXPORT FlowGenerator {
    // Restrict this template for using only types derived from edge Interfaces
    BOOST_STATIC_ASSERT((boost::is_base_of<Edge, EdgeType>::value));

public:
    typedef std::unordered_set<std::shared_ptr<EdgeType>, EdgeHash<EdgeType>> EdgesSet;

    FlowGenerator();
    virtual ~FlowGenerator();

    Flow<EdgeType> makePossibleFlowsBacktraking() throw(std::runtime_error);
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
Flow<EdgeType> FlowGenerator<EdgeType>::makePossibleFlowsBacktraking() throw(std::runtime_error){
    EdgesSet tempSet(edgesSet);

    bool finded = false;
    std::vector<std::shared_ptr<EdgeType>> calculatedFlow;
    for (auto it = edgesSet.begin(); !finded && (it != edgesSet.end()); ++it) {
        std::shared_ptr<EdgeType> initFlowEdge = *it;

        //add solution
        auto itTemp = tempSet.find(initFlowEdge);
        tempSet.erase(itTemp);
        calculatedFlow.push_back(initFlowEdge);

        //recursive call
        finded = makePossibleFlows_recursive(calculatedFlow, initFlowEdge, tempSet);

        //remove solution if failure
        if (!finded) {
            tempSet.insert(initFlowEdge);
            calculatedFlow.pop_back();
        }
    }

    if (finded) {
        //construct solution
        int idSource = calculatedFlow.at(0)->getIdSource();
        int idTarget = calculatedFlow.back()->getIdTarget();
        Flow<EdgeType> flow(idSource, idTarget, calculatedFlow);

        return flow;
    } else {
        std::string edgesSetStr = "";
        for (std::shared_ptr<EdgeType> edge: edgesSet) {
            edgesSetStr += edge->toText();
        }
        throw (std::runtime_error("imposible to make an unique flow with all the edges in the generator: " + edgesSetStr));
    }
}

template<class EdgeType>
void FlowGenerator<EdgeType>::addEdge(std::shared_ptr<EdgeType> edge) {
    edgesSet.insert(edge);
}

template<class EdgeType>
void FlowGenerator<EdgeType>::removeEdge(std::shared_ptr<EdgeType> edge) {
    auto it = edgesSet.find(edge);
    edgesSet.erase(it);
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
            std::shared_ptr<EdgeType> actualFlowEdge = *it;

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
                    tempSet.insert(actualFlowEdge);
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
