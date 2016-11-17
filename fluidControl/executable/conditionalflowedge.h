#ifndef CONDITIONALFLOWEDGE_H
#define CONDITIONALFLOWEDGE_H

#include "graph/Edge.h"
#include "util/Patch.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>

class ConditionalFlowEdge : public Edge
{
public:
    //derived of edge
    ConditionalFlowEdge();
    ConditionalFlowEdge(const ConditionalFlowEdge & edge);
    //
    ConditionalFlowEdge(int idSource, int idTarget, const std::vector<std::shared_ptr<ConditionalFlowEdge>> & allowedPreviousEdge);

    virtual ~ConditionalFlowEdge();

    virtual std::string toText();

    //SERIALIZATIoN
    template<class Archive>
    void serialize(Archive & ar, std::uint32_t const version);

protected:
    std::vector<std::shared_ptr<ConditionalFlowEdge>> allowedPreviousEdge;
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
