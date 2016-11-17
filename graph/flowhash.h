#ifndef FLOWHASH_H
#define FLOWHASH_H

// boost library
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>

//local
#include "graph/Edge.h"
#include "graph/Flow.h"
#include "util/Utils.h"

template<class EdgeType> class  FlowHash {
    // Restrict this template for using only types derived from edge Interfaces
    BOOST_STATIC_ASSERT((boost::is_base_of<Edge, EdgeType>::value));

public:
    FlowHash(){}
    virtual ~FlowHash(){}

    inline size_t operator() (const Flow<EdgeType> & flow) const {
        return this->makeHash(flow);
    }

    inline size_t operator() (Flow<EdgeType>* flow) const {
        return this->makeHash(*flow);
    }

    inline size_t operator() (std::shared_ptr<Flow<EdgeType>> flow) const {
        return this->makeHash(*flow.get());
    }

    inline bool operator() (const Flow<EdgeType> & flow1, const Flow<EdgeType> & flow2) const {
        return (makeHash(flow1) == makeHash(flow2));
    }

    inline bool operator() (Flow<EdgeType>* flow1, Flow<EdgeType>* flow2) const {
        return (makeHash(*flow1) == makeHash(*flow2));
    }

    inline bool operator() (std::shared_ptr<Flow<EdgeType>> flow1, std::shared_ptr<Flow<EdgeType>> flow2) const {
        return (makeHash(*flow1.get()) == makeHash(*flow2.get()));
    }
protected:
    inline size_t makeHash(const Flow<EdgeType> & flow) const {
        size_t hash = 0;

        std::string stack = "";
        size_t size = flow.getPaths().size();
        for (int i = 0; i < size; i++) {
            std::shared_ptr<EdgeType> node = flow.getPaths().at(i);

            stack += patch::to_string(node->getIdSource());
            if (i == (size - 1)) {
                stack += patch::to_string(node->getIdTarget());
            }
        }

        hash = Utils::strToSizeT(stack);
        return hash;
    }
};

#endif // FLOWHASH_H
