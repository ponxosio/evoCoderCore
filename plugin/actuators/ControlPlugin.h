#pragma once

#pragma warning( disable : 4290 )

#include <string>
#include <vector>
#include <stdexcept>
#include <array>

//local
#include "fluidControl\executable\containers\actuators\liquids\Control.h"
#include "fluidControl\executable\containers\actuators\communications\CommunicationsInterface.h"
#include "plugin\PythonEnvironment.h"
#include "selfconfiguringplugin.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/array.hpp>

#include "evocodercore_global.h"

class CONTROLPLUGIN_EXPORT ControlPlugin :
    public Control, public SelfConfiguringPlugin
{
public:
	ControlPlugin();
    ControlPlugin(int communications, const std::string & name, const std::string & pluginType,
        const std::unordered_map<std::string,std::string> & params);

	virtual ~ControlPlugin();

    virtual void addConnection(int idSource, int idTarget, int pos) throw (std::runtime_error);
	virtual void setConnection(int idSource, int idTarget) throw (std::runtime_error);
	virtual void clearConnections() throw (std::runtime_error);
	virtual std::string getInstructions() throw (std::runtime_error);
    virtual std::vector<int> getAvailablePos() throw (std::runtime_error);
    virtual void reloadConnections() throw (std::runtime_error);
    virtual int getMaxConnections() throw (std::runtime_error);
    virtual int getActualPosition() throw (std::runtime_error);

    inline virtual SelfConfiguringPlugin* clone() {
        return new ControlPlugin();
    }

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);

protected:
	std::string referenceName;
    std::unordered_map<int, std::array<int, 2>> posMap;
};



template<class Archive>
inline void ControlPlugin::serialize(Archive& ar,
	const std::uint32_t version) {
	if (version <= 1) {
		Control::serialize(ar, version);
        SelfConfiguringPlugin::serialize(ar, version);
        ar(CEREAL_NVP(posMap));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION(ControlPlugin, (int)1);

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(ControlPlugin, "ControlPlugin");
