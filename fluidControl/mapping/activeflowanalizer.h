#ifndef ACTIVEFLOWANALIZER_H
#define ACTIVEFLOWANALIZER_H

#include <unordered_set>

//local
#include "graph/Flow.h"
#include "protocolGraph/ProtocolGraph.h"

class ActiveFlowAnalizer
{
public:
    ActiveFlowAnalizer();

protected:
    std::shared_ptr<ProtocolGraph> protocol;

};

#endif // ACTIVEFLOWANALIZER_H
