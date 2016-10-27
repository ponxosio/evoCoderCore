#include "compoundcontrolplugin.h"

CompoundControlPlugin::CompoundControlPlugin():
    Control(), SelfConfiguringPlugin()
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

    for (auto actualPair = virtualPosMap.begin(); actualPair != virtualPosMap.end() && !findedPlugin; ++actualPair) {
        if (pos < actualPair->first) {
            translatedPos = pos - lastMaxVirtualPos;
            findedPlugin = actualPair->second;
        } else {
            lastMaxVirtualPos = actualPair->first;
        }
    }
    findedPlugin->addConnection(idSource, idTarget, translatedPos);

    auto element = std::find(availableVirtualPos.begin(), availableVirtualPos.end(), pos);
    availableVirtualPos.erase(element);
}

void CompoundControlPlugin::setConnection(int idSource, int idTarget) throw (std::runtime_error) {
    for (auto actualPair = virtualPosMap.begin(); actualPair != virtualPosMap.end(); ++actualPair) {
        actualPair->second->setConnection(idSource, idTarget);
    }
}

void CompoundControlPlugin::clearConnections() throw (std::runtime_error) {
    availableVirtualPos.clear();
    availableVirtualPos.reserve(virtualPos.size());
    std::copy(availableVirtualPos.begin(), availableVirtualPos.end(), virtualPos.begin());

    for (auto actualPair = virtualPosMap.begin(); actualPair != virtualPosMap.end(); ++actualPair) {
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
    for (auto actualPair = virtualPosMap.begin(); actualPair != virtualPosMap.end(); ++actualPair) {
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
