#ifndef COMPOUNDCONTROLPLUGIN_H
#define COMPOUNDCONTROLPLUGIN_H

//std
#include <vector>
#include <map>
#include <algorithm>

//boost
#include <boost/range/irange.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

//local
#include "fluidControl/executable/containers/actuators/liquids/Control.h"
#include "plugin/actuators/ControlPlugin.h"
#include "plugin/actuators/selfconfiguringplugin.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>

//dll
#include "evocodercore_global.h"

class COMPOUNDCONTROLPLUGIN_EXPORT CompoundControlPlugin : public Control, public SelfConfiguringPlugin
{
public:
    CompoundControlPlugin();
    CompoundControlPlugin(int communications,const std::unordered_map<std::string, std::string> & params);

    virtual ~CompoundControlPlugin();

    //Control pure virtual methods
    virtual void addConnection(int idSource, int idTarget, int pos) throw (std::runtime_error);
    virtual void setConnection(int idSource, int idTarget) throw (std::runtime_error);
    virtual void clearConnections() throw (std::runtime_error);
    virtual std::string getInstructions() throw (std::runtime_error);
    virtual std::vector<int> getAvailablePos() throw (std::runtime_error);
    virtual void reloadConnections() throw (std::runtime_error);
    virtual int getMaxConnections() throw (std::runtime_error);
    virtual int getActualPosition() throw (std::runtime_error);

    //selfconfiguring plugin overriden method
    virtual std::vector<std::pair<std::string,std::string>> getParamsType() throw (std::runtime_error);

    //compoundControl own methods
    void groupValves(const std::vector<std::shared_ptr<ControlPlugin>> & controls);

    //selfconfiguring plugin pure virtual method
    inline virtual SelfConfiguringPlugin* clone() {
        return new CompoundControlPlugin();
    }

    //SERIALIZATIoN
    template<class Archive>
    void serialize(Archive & ar, std::uint32_t const version);

protected:
    std::map<int, std::shared_ptr<ControlPlugin>> virtualPosMap;
    std::vector<int> virtualPos;
    std::vector<int> availableVirtualPos;
    int maxConnections;
};

template<class Archive>
inline void CompoundControlPlugin::serialize(Archive& ar,
    const std::uint32_t version) {
    if (version <= 1) {
        Control::serialize(ar, version);
        SelfConfiguringPlugin::serialize(ar, version);
        ar(CEREAL_NVP(virtualPosMap),
           CEREAL_NVP(virtualPos),
           CEREAL_NVP(availableVirtualPos),
           CEREAL_NVP(maxConnections));
    }
}

// Associate some type with a version number
CEREAL_CLASS_VERSION(CompoundControlPlugin, (int)1);

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(CompoundControlPlugin, "CompoundControlPlugin");

#endif // COMPOUNDCONTROLPLUGIN_H
