#ifndef CONDITIONALFLOWEDGE_H
#define CONDITIONALFLOWEDGE_H

#include "unordered_set"

//local
#include "graph/Edge.h"
#include "graph/edgehash.h"
#include "util/Patch.h"

#include "evocodercore_global.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>

class CONDITIONALFLOWEDGE_EXPORT ConditionalFlowEdge : public Edge
{
public:
    typedef std::unordered_set<std::shared_ptr<ConditionalFlowEdge>, EdgeHash<ConditionalFlowEdge>, EdgeHash<ConditionalFlowEdge>> AllowedEdgeSet;

    //derived of edge
    ConditionalFlowEdge();
    ConditionalFlowEdge(const ConditionalFlowEdge & edge);
    //
    ConditionalFlowEdge(int idSource, int idTarget, const AllowedEdgeSet & allowedPreviousEdge);

    virtual ~ConditionalFlowEdge();

    virtual std::string toText();

    inline const AllowedEdgeSet & getAllowedPreviousEdges() {
        return allowedPreviousEdge;
    }

    //SERIALIZATIoN
    template<class Archive>
    void serialize(Archive & ar, std::uint32_t const version);

protected:
    AllowedEdgeSet allowedPreviousEdge;
};

template<class Archive>
inline void ConditionalFlowEdge::serialize(Archive& ar, const std::uint32_t version) {
    if (version <= 1) {
        Edge::serialize(ar, version);
        ar(CEREAL_NVP(allowedPreviousEdge));
    }
}

// Associate some type with a version number
CEREAL_CLASS_VERSION(ConditionalFlowEdge, (int)1);

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(ConditionalFlowEdge, "ConditionalFlowEdge");

#endif // CONDITIONALFLOWEDGE_H
