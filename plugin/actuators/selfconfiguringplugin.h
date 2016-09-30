#ifndef SELFCONFIGURINGPLUGIN_H
#define SELFCONFIGURINGPLUGIN_H

# include <vector>
# include <unordered_map>
# include <stdexcept>

//Qt
#include <QDebug>

//lib
#include <boost/python.hpp>

//local
#include "..\PythonEnvironment.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>

class SelfConfiguringPlugin
{
public:
    static std::vector<std::pair<std::string,std::string>> getParamsType(const std::string & pluginType) throw (std::runtime_error);

    SelfConfiguringPlugin();
    SelfConfiguringPlugin(const std::string & pluginType, const std::unordered_map<std::string,std::string> & params);
    virtual ~SelfConfiguringPlugin();

    //setters & getters
    void setPluginType(const std::string & pluginType);
    void setParams(const std::unordered_map<std::string,std::string> & params);
    std::string getPluginType();
    const std::unordered_map<std::string,std::string> & getParams();

    virtual SelfConfiguringPlugin* clone() = 0;

    //SERIALIZATIoN
    template<class Archive>
    void serialize(Archive & ar, std::uint32_t const version);

protected:
    std::string pluginType;
    std::unordered_map<std::string,std::string> params;
};

template<class Archive>
inline void SelfConfiguringPlugin::serialize(Archive& ar,
    const std::uint32_t version) {
    if (version <= 1) {
        //SelfConfiguringPlugin::serialize(ar, version);
        ar(CEREAL_NVP(pluginType), CEREAL_NVP(params));
    }
}

// Associate some type with a version number
CEREAL_CLASS_VERSION(SelfConfiguringPlugin, (int)1);

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(SelfConfiguringPlugin, "SelfConfiguringPlugin");

#endif // SELFCONFIGURINGPLUGIN_H
