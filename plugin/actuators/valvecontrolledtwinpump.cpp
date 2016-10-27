#include "valvecontrolledtwinpump.h"

ValveControlledTwinPump::ValveControlledTwinPump() :
    Extractor(), SelfConfiguringPlugin()
{

}

ValveControlledTwinPump::ValveControlledTwinPump(int communications,
        const std::unordered_map<std::string,std::string> & params,
        std::shared_ptr<ExtractorPlugin> pump1,
        std::shared_ptr<ExtractorPlugin> pump2,
        std::shared_ptr<ControlPlugin> controlActuator,
        const std::vector<int> & positionsPump1Works) :
    Extractor(communications), SelfConfiguringPlugin("valveControlledTwinPump", params)
{
    this->pump1 = pump1;
    this->pump2 = pump2;
    this->controlActuator = controlActuator;
    this->positionsPump1Works.reserve(positionsPump1Works.size());
    std::copy(positionsPump1Works.begin(), positionsPump1Works.end(), this->positionsPump1Works.begin());
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
