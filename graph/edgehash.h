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
    BOOST_STATIC_ASSERT((boost::is_base_of<Edge, EdgeType>::value));

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
        return Utils::cantorParingFunctions(edge.getIdSource(), edge.getIdTarget());
    }
};

#endif // EDGECOMPARATOR_H
