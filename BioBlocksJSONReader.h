#pragma once

# include <fstream>
# include <unordered_map>

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

	ProtocolGraph* loadFile(const std::string & path);

private:
	static BioBlocksJSONReader* m_pInstance;

	std::unordered_map<std::string, int> containerMap;
	AutoEnumerate table_sequence;
	AutoEnumerate graph_sequence;

	BioBlocksJSONReader();
    BioBlocksJSONReader(const BioBlocksJSONReader &){}
    BioBlocksJSONReader& operator=(const BioBlocksJSONReader & com){return BioBlocksJSONReader(com);}

	float parseVolume(const std::string & text);
};

