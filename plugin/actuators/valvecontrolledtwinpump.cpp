#include "valvecontrolledtwinpump.h"

ValveControlledTwinPump::ValveControlledTwinPump() :
    Extractor(), SelfConfiguringPlugin()
{

}

ValveControlledTwinPump::ValveControlledTwinPump(int communications,
        const std::string & name,
        const std::unordered_map<std::string,std::string> & params,
        std::shared_ptr<ExtractorPlugin> pump1,
        std::shared_ptr<ExtractorPlugin> pump2,
        std::shared_ptr<ControlPlugin> controlActuator,
        const std::vector<int> & positionsPump1Works) :
    Extractor(communications), SelfConfiguringPlugin(name, "valveControlledTwinPump", params)
{
    this->pluginType = "valveControlledTwinPump";
    this->pump1 = pump1;
    this->pump2 = pump2;
    this->controlActuator = controlActuator;

    for (int pos : positionsPump1Works) {
        this->positionsPump1Works.push_back(pos);
    }
}

ValveControlledTwinPump::~ValveControlledTwinPump() {

}

//extractor pure virtual methods
void ValveControlledTwinPump::extractLiquid(double rate) throw(std::runtime_error) {
    int actualPos = controlActuator->getActualPosition();
    auto finded = std::find(positionsPump1Works.begin(), positionsPump1Works.end(), actualPos);

    if (finded != positionsPump1Works.end()) {
        pump1->extractLiquid(rate);
    } else {
        pump2->extractLiquid(rate);
    }
}

std::string ValveControlledTwinPump::getInstructions() throw (std::runtime_error) {
    return "";
}

int ValveControlledTwinPump::getMovementType() throw (std::runtime_error) {
    return pump1->getMovementType();
}

//selfconfiguringplugin pure virtual method
SelfConfiguringPlugin* ValveControlledTwinPump::clone() {
    return new ValveControlledTwinPump();
}

void ValveControlledTwinPump::setCommunications(int communications) {
    pump1->setCommunications(communications);
    pump2->setCommunications(communications);
    controlActuator->setCommunications(communications);
}

void ValveControlledTwinPump::setPluginType(const std::string & pluginType) {
    this->pluginType = "valveControlledTwinPump";
}
