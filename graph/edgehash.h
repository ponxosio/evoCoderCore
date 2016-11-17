#ifndef EDGECOMPARATOR_H
#define EDGECOMPARATOR_H

// boost library
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>

//local
# include "graph/Edge.h"
# include "util/Utils.h"

template<class EdgeType> class EdgeHash {
    // Restrict this template for using only types derived from edge Interfaces
    //BOOST_STATIC_ASSERT((boost::is_base_of<Edge, EdgeType>::value));

public:
    EdgeHash() {}
    virtual ~EdgeHash(){}

    inline size_t operator()(EdgeType* edge) const {
        return Utils::cantorParingFunction(edge->getIdSource(), edge->getIdTarget());
    }

    inline size_t operator()(std::shared_ptr<EdgeType> edge) const {
        return Utils::cantorParingFunction(edge->getIdSource(), edge->getIdTarget());
    }

    inline size_t operator()(const EdgeType & edge) const {
        return Utils::cantorParingFunction(edge.getIdSource(), edge.getIdTarget());
    }

    inline bool operator()(EdgeType* edge1, EdgeType* edge2) const {
        return (Utils::cantorParingFunction(edge1->getIdSource(), edge1->getIdTarget()) ==
                Utils::cantorParingFunction(edge2->getIdSource(), edge2->getIdTarget()));
    }

    inline bool operator()(std::shared_ptr<EdgeType> edge1, std::shared_ptr<EdgeType> edge2) const {
        return (Utils::cantorParingFunction(edge1->getIdSource(), edge1->getIdTarget()) ==
                Utils::cantorParingFunction(edge2->getIdSource(), edge2->getIdTarget()));
    }

    inline bool operator()(const EdgeType & edge1, const EdgeType & edge2) const {
        return (Utils::cantorParingFunction(edge1.getIdSource(), edge1.getIdTarget()) ==
                Utils::cantorParingFunction(edge2.getIdSource(), edge2.getIdTarget()));
    }
};

#endif // EDGECOMPARATOR_H
