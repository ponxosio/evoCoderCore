#include "conditionalflowedge.h"

ConditionalFlowEdge::ConditionalFlowEdge() :
    Edge()
{

}

ConditionalFlowEdge::ConditionalFlowEdge(const ConditionalFlowEdge & edge) :
    Edge(edge)
{
    this->allowedPreviousEdge = AllowedEdgeSet(edge.allowedPreviousEdge);
}

//
ConditionalFlowEdge::ConditionalFlowEdge(int idSource, int idTarget, const AllowedEdgeSet & allowedPreviousEdge) :
    Edge(idSource, idTarget)
{
    this->allowedPreviousEdge = AllowedEdgeSet(allowedPreviousEdge);
}

ConditionalFlowEdge::~ConditionalFlowEdge() {

}

std::string ConditionalFlowEdge::toText() {
    std::string allowedStr = "";
    for (std::shared_ptr<ConditionalFlowEdge> edge: allowedPreviousEdge) {
        allowedStr += patch::to_string(edge->idSource) + "->" + patch::to_string(edge->idTarget) + ";";
    }
    return patch::to_string(idSource) + "->" + patch::to_string(idTarget) + (!allowedStr.empty() ? "[label=\"" + allowedStr + "\"];" : ";");
}
