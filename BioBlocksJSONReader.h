#pragma once
#pragma warning( disable : 4290 )

#include <stdexcept>
#include <fstream>
#include <unordered_map>

#include <boost/python.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>

//lib
# include <json/json.hpp>

//local
# include "operables\comparison\ComparisonOperable.h"
# include "operables\comparison\Tautology.h"
# include "operables\mathematics\MathematicOperable.h"
# include "operables\mathematics\ConstantNumber.h"
# include "protocolGraph\ProtocolGraph.h"
# include "protocolGraph\OperationNode.h"
# include "protocolGraph\operations\container\LoadContainerOperation.h"
# include "protocolGraph\operations\container\Transfer.h"
# include "protocolGraph\operations\container\SetContinousFlow.h"
# include "util\AutoEnumerate.h"
# include "util\Patch.h"

#include "evocodercore_global.h"

class BIOBLOCKSJSONREADER_EXPORT BioBlocksJSONReader
{
public:

	static BioBlocksJSONReader* GetInstance() {
		if (!m_pInstance)
			m_pInstance = new BioBlocksJSONReader();
		return m_pInstance;
	}
	static void freeCommandInterface();

	virtual ~BioBlocksJSONReader();

    ProtocolGraph* loadFile(const std::string & path) throw(std::invalid_argument);
protected:
    std::vector<std::shared_ptr<OperationNode>> lastOp;
    std::unordered_map<std::string, int> containerMap;
    AutoEnumerate table_sequence;
    AutoEnumerate graph_sequence;
    std::shared_ptr<ComparisonOperable> tautology;

    void readRefs(const nlohmann::json & js, ProtocolGraph* protocol) throw(std::invalid_argument);
    void readInstructions(const nlohmann::json & js, ProtocolGraph* protocol);

    void parsePipette(const nlohmann::json::iterator & it, ProtocolGraph* protocol);
    void parsePipette_one2many(const std::string & key, nlohmann::json to, nlohmann::json from, ProtocolGraph* protocol);
    void parsePipette_many2one(const std::string & key, nlohmann::json to, nlohmann::json from, nlohmann::json value, ProtocolGraph* protocol);
    void parsePipette_one2one(const std::string & key, nlohmann::json to, nlohmann::json from, nlohmann::json value, ProtocolGraph* protocol);

    float parseVolume(const std::string & text);


private:
	static BioBlocksJSONReader* m_pInstance;

	BioBlocksJSONReader();
    BioBlocksJSONReader(const BioBlocksJSONReader &){}
    BioBlocksJSONReader& operator=(const BioBlocksJSONReader & com){return BioBlocksJSONReader(com);}
};

