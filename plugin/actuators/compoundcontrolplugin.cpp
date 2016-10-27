#include "compoundcontrolplugin.h"

CompoundControlPlugin::CompoundControlPlugin():
    Control(), SelfConfiguringPlugin("compoundControl", CompoundControlPlugin::generateParams())
{
    maxConnections = 0;
}

CompoundControlPlugin::CompoundControlPlugin(int communications, const std::unordered_map<std::string, std::string> & params) :
    Control(communications), SelfConfiguringPlugin("compoundControl", params)
{
    maxConnections = 0;
}

CompoundControlPlugin::~CompoundControlPlugin() {

}

//Control pure virtual methods
void CompoundControlPlugin::addConnection(int idSource, int idTarget, int pos) throw (std::runtime_error) {
    std::shared_ptr<ControlPlugin> findedPlugin;
    int lastMaxVirtualPos = 0;
    int translatedPos = -1;

    for (auto actualPair = virtualPos.begin(); actualPair != virtualPos.end() && !findedPlugin; ++actualPair) {
        if (pos < actualPair->firts) {
            translatedPos = pos - lastMaxVirtualPos;
            findedPlugin = actualPair->second;
        } else {
            lastMaxVirtualPos = actualPair->firts;
        }
    }
    findedPlugin->addConnection(idSource, idTarget, translatedPos);

    auto element = std::find(availableVirtualPos.begin(), availableVirtualPos.end(), pos);
    availableVirtualPos.erase(element);
}

void CompoundControlPlugin::setConnection(int idSource, int idTarget) throw (std::runtime_error) {
    for (auto actualPair = virtualPos.begin(); actualPair != virtualPos.end(); ++actualPair) {
        actualPair->second->setConnection(idSource, idTraget);
    }
}

void CompoundControlPlugin::clearConnections() throw (std::runtime_error) {
    availableVirtualPos = std::vector(virtualPos);
    for (auto actualPair = virtualPos.begin(); actualPair != virtualPos.end(); ++actualPair) {
        actualPair->second->clearConnections();
    }
}

std::string CompoundControlPlugin::getInstructions() throw (std::runtime_error) {
    return "";
}

std::vector<int> CompoundControlPlugin::getAvailablePos() throw (std::runtime_error) {
    return availableVirtualPos;
}

void CompoundControlPlugin::reloadConnections() throw (std::runtime_error) {
    for (auto actualPair = virtualPos.begin(); actualPair != virtualPos.end(); ++actualPair) {
        actualPair->second->clearConnections();
    }
}

int CompoundControlPlugin::getMaxConnections() throw (std::runtime_error) {
    return this->maxConnections;
}

//compund control own method
void CompoundControlPlugin::groupValves(const std::vector<std::shared_ptr<ControlPlugin>> & controls) {
    virtualPos.clear();
    availableVirtualPos.clear();
    virtualPosMap.clear();

    int maxConnection = 0;
    for (std::shared_ptr<ControlPlugin> actual : controls) {
        int numConnections = actual->getMaxConnections();

        boost::push_back(availableVirtualPos, boost::irange(maxConnection, maxConnection + numConnections));
        boost::push_back(virtualPos, boost::irange(maxConnection, maxConnection + numConnections));

        maxConnection += numConnections;
        virtualPosMap.insert(std::make_pair(maxConnection, actual));
    }
    this->maxConnections = maxConnection;
}

int CompoundControlPlugin::getActualPosition() throw (std::runtime_error) {
    return -1;
}

//selfconfiguring plugin overriden method
std::vector<std::pair<std::string,std::string>> CompoundControlPlugin::getParamsType() throw (std::runtime_error) {
    std::vector<std::pair<std::string,std::string>> paramsType = {std::make_pair("list_of_controls", "list[plugin]")};
    return paramsType;
}
