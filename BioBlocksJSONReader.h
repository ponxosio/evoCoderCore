#pragma once
#pragma warning( disable : 4290 )

#include <stdexcept>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>

#include <boost/python.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>

//lib
# include <json/json.hpp>

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

# include "operables/mathematics/ConstantNumber.h"
# include "operables/mathematics/MathematicOperable.h"
# include "operables/mathematics/VariableEntry.h"

# include "operables/comparison/ComparisonOperable.h"
# include "operables/comparison/Tautology.h"
# include "operables/comparison/SimpleComparison.h"
# include "operables/comparison/BooleanComparison.h"

# include "util/AutoEnumerate.h"
# include "util/Patch.h"

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

typedef struct PipetteOperationComparator_ {
    bool operator()(PipetteOperation f1, PipetteOperation f2) const {
        return (f1.timeOfOperation < f2.timeOfOperation);
    }
} PipetteOperationComparator;

}

class BIOBLOCKSJSONREADER_EXPORT BioBlocksJSONReader
{
public:
    typedef std::priority_queue<BioBlocks::PipetteOperation, std::vector<BioBlocks::PipetteOperation>, BioBlocks::PipetteOperationComparator> OperationHeap;

    static BioBlocksJSONReader* GetInstance() {
        if (!m_pInstance)
            m_pInstance = new BioBlocksJSONReader();
        return m_pInstance;
    }

    static void freeCommandInterface();
    static void setTimeSlice(long timeSlice);


    virtual ~BioBlocksJSONReader();

    ProtocolGraph* loadFile(const std::string & path) throw(std::invalid_argument);

    float parseVolume(const std::string & text);
    float parseTime(const std::string & text);
    float parseFlowRate(const std::string & text) throw (std::invalid_argument);

protected:
    static long timeSlice;

    OperationHeap operationHeap;
    std::vector<std::shared_ptr<OperationNode>> lastOp;
    std::unordered_map<std::string, int> containerMap;
    AutoEnumerate table_sequence;
    AutoEnumerate graph_sequence;
    std::shared_ptr<ComparisonOperable> tautology;
    long finishTime;

    void readRefs(const nlohmann::json & js, ProtocolGraph* protocol) throw(std::invalid_argument);
    void readInstructions(const nlohmann::json & js, OperationHeap & operationHeap);
    void parseHeap(OperationHeap & operationHeap, ProtocolGraph* protocol);

    void parsePipette(const nlohmann::json::iterator & it, OperationHeap & operationHeap);
    void parsePipette_one2many(const std::string & key, nlohmann::json to, nlohmann::json from, float timeInit, float duration, OperationHeap & operationHeap);
    void parsePipette_many2one(const std::string & key, nlohmann::json to, nlohmann::json from, nlohmann::json value, float timeInit, float duration, OperationHeap & operationHeap);
    void parsePipette_one2one(const std::string & key, nlohmann::json to, nlohmann::json from, nlohmann::json value, float timeInit, float duration, OperationHeap & operationHeap);

    float toMillilitersPower(std::string unit) throw (std::invalid_argument);
    float toMillisecondsPower(std::string unit) throw (std::invalid_argument);

    std::shared_ptr<OperationNode> createIfBlock(long initTime, long duration, shared_ptr<ComparisonOperable> & inCompare, shared_ptr<ComparisonOperable> & outCompare);

    std::shared_ptr<OperationNode> toOperationNode(const BioBlocks::PipetteOperation & op);
private:
    //*** SINGLETON ***
    static BioBlocksJSONReader* m_pInstance;

    BioBlocksJSONReader();
    BioBlocksJSONReader(const BioBlocksJSONReader &){}
    BioBlocksJSONReader& operator=(const BioBlocksJSONReader & com){return BioBlocksJSONReader(com);}
    // ****************
};
