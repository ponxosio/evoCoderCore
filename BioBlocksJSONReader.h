#pragma once
#pragma warning( disable : 4290 )

#include <stdexcept>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <tuple>

#include <boost/python.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>

//lib
# include <json/json.hpp>
# include <easylogging++.h>

//local
# include "protocolGraph/ProtocolGraph.h"
# include "protocolGraph/OperationNode.h"

//# include "protocolGraph/operations/container/ContainerOperation.h"
# include "protocolGraph/operations/container/LoadContainerOperation.h"
# include "protocolGraph/operations/container/SetContinousFlow.h"
# include "protocolGraph/operations/container/Transfer.h"
# include "protocolGraph/operations/container/TimeStep.h"
# include "protocolGraph/operations/DivergeNode.h"
# include "protocolGraph/operations/LoopNode.h"
# include "protocolGraph/operations/AssignationOperation.h"
# include "protocolGraph/operations/container/settimestep.h"

# include "operables/mathematics/ConstantNumber.h"
# include "operables/mathematics/MathematicOperable.h"
# include "operables/mathematics/VariableEntry.h"

# include "operables/comparison/ComparisonOperable.h"
# include "operables/comparison/Tautology.h"
# include "operables/comparison/SimpleComparison.h"
# include "operables/comparison/BooleanComparison.h"

# include "util/AutoEnumerate.h"
# include "util/Patch.h"
# include "util/Utils.h"

#include "evocodercore_global.h"

namespace BioBlocks {

struct PipetteOperation {
    long timeOfOperation;
    long duration;
    int source;
    int destination;
    float rate;
    bool continuos;

    PipetteOperation(long timeOfOperation, long duration, int source, int destination, float rate, bool continuos) :
        timeOfOperation(timeOfOperation),
        duration(duration),
        source(source),
        destination(destination),
        rate(rate),
        continuos(continuos) {}
};
}

class BIOBLOCKSJSONREADER_EXPORT BioBlocksJSONReader
{
public:
    typedef std::map<std::tuple<long,long>, std::vector<BioBlocks::PipetteOperation>, PairLongLongCompareFunction> OperationsMultiMap;

    BioBlocksJSONReader(const std::string & path, long timeSlice) throw(std::invalid_argument);
    virtual ~BioBlocksJSONReader();

    std::shared_ptr<ProtocolGraph> getProtocol();

    float parseVolume(const std::string & text);
    float parseTime(const std::string & text);
    float parseFlowRate(const std::string & text) throw (std::invalid_argument);
protected:
    std::shared_ptr<ProtocolGraph> protocol;
    std::unordered_map<std::string, int> containerMap;

    OperationsMultiMap operationMap;
    std::vector<std::shared_ptr<OperationNode>> lastOp;

    AutoEnumerate table_sequence;
    AutoEnumerate graph_sequence;
    long finishTime;
    long timeSlice;

    std::shared_ptr<ComparisonOperable> tautology;
    std::shared_ptr<VariableEntry> time;

    void loadFile(const std::string & path) throw(std::invalid_argument);

    void readRefs(const nlohmann::json & js) throw(std::invalid_argument);
    void readInstructions(const nlohmann::json & js);

    void parseHeap();
    ProtocolGraph::ProtocolNodePtr parseHeap_initLoop(std::shared_ptr<ComparisonOperable> & lastComp);
    void parseHeap_processOpBlock (int numBlock,
                                   long initTime,
                                   long duration,
                                   const std::vector<BioBlocks::PipetteOperation> & sameBlockOps,
                                   std::shared_ptr<OperationNode> & lastIfStart,
                                   std::shared_ptr<ComparisonOperable> & lastStartComp,
                                   std::shared_ptr<OperationNode> & lastIfStop,
                                   std::shared_ptr<ComparisonOperable> & lastStopComp,
                                   std::shared_ptr<ComparisonOperable> & lastComp);
    void parseHeap_createInternalVariables();
    std::shared_ptr<OperationNode> createIfBlock(long initTime, long duration, shared_ptr<ComparisonOperable> & inCompare, shared_ptr<ComparisonOperable> & outCompare);

    void parsePipette(const nlohmann::json::iterator & it);
    void parsePipette_one2many(const std::string & key, nlohmann::json to, nlohmann::json from, float timeInit, float duration);
    void parsePipette_many2one(const std::string & key, nlohmann::json to, nlohmann::json from, float timeInit, float duration);
    void parsePipette_one2one(const std::string & key, nlohmann::json to, nlohmann::json from, nlohmann::json value, float timeInit, float duration);

    void storeOp(const BioBlocks::PipetteOperation & op);

    float toMillilitersPower(std::string unit) throw (std::invalid_argument);
    float toMillisecondsPower(std::string unit) throw (std::invalid_argument);

    std::shared_ptr<OperationNode> toOperationNode(const BioBlocks::PipetteOperation & op);
};
