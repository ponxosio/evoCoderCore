#ifndef VALVECONTROLLEDTWINPUMP_H
#define VALVECONTROLLEDTWINPUMP_H

//stl
#include <algorithm>

#include <easylogging++.h>

//local
#include "fluidControl/executable/containers/actuators/liquids/Extractor.h"
#include "fluidControl/executable/containers/actuators/liquids/Control.h"
#include "plugin/actuators/ControlPlugin.h"
#include "plugin/actuators/ExtractorPlugin.h"

#include "plugin/actuators/selfconfiguringplugin.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

//dll
#include "evocodercore_global.h"

class VALVECONTROLLEDTWINPUMP_EXPORT ValveControlledTwinPump : public SelfConfiguringPlugin, public Extractor
{
public:
    ValveControlledTwinPump();
    ValveControlledTwinPump(int communications,
            const std::string & name,
            const std::unordered_map<std::string,std::string> & params,
            std::shared_ptr<ExtractorPlugin> pump1,
            std::shared_ptr<ExtractorPlugin> pump2,
            std::shared_ptr<ControlPlugin> controlActuator,
            const std::vector<int> & positionsPump1Works);

    virtual ~ValveControlledTwinPump();

    //extractor pure virtual method
    virtual void extractLiquid(double rate) throw(std::runtime_error);
    virtual std::string getInstructions() throw (std::runtime_error);
    virtual int getMovementType() throw (std::runtime_error);

    //extractor overriden methods
    virtual void setCommunications(int communications);

    //selfconfiguringplugin pure virtual method
    virtual SelfConfiguringPlugin* clone();

    //selfconfigurationplugin overriden methods
    virtual void setPluginType(const std::string & pluginType);

    //SERIALIZATIoN
    template<class Archive>
    void serialize(Archive & ar, std::uint32_t const version);

protected:
    std::shared_ptr<ExtractorPlugin> pump1;
    std::shared_ptr<ExtractorPlugin> pump2;
    std::shared_ptr<ControlPlugin> controlActuator;
    std::vector<int> positionsPump1Works;
};

template<class Archive>
inline void ValveControlledTwinPump::serialize(Archive& ar,
    const std::uint32_t version) {
    if (version <= 1) {
        //Extractor::serialize(ar, version);
        //SelfConfiguringPlugin::serialize(ar, version);
       /* ar(CEREAL_NVP(pump1),
           CEREAL_NVP(pump2),
           CEREAL_NVP(controlActuator),
           CEREAL_NVP(positionsPump1Works));*/
    }
}

// Associate some type with a version number
CEREAL_CLASS_VERSION(ValveControlledTwinPump, (int)1);

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(ValveControlledTwinPump, "ValveControlledTwinPump");

#endif // VALVECONTROLLEDTWINPUMP_H
