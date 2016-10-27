# include "selfconfiguringplugin.h"

using namespace boost::python;

SelfConfiguringPlugin::SelfConfiguringPlugin()  {
    this->pluginType = "unknow";
}

SelfConfiguringPlugin::SelfConfiguringPlugin(const std::string & pluginType, const std::unordered_map<std::string,std::string> & params) {
    this->pluginType = std::string(pluginType);

    for (auto it: params) {
        this->params.insert(std::make_pair(it.first, it.second));
    }
}

SelfConfiguringPlugin::~SelfConfiguringPlugin() {

}


//getters & setter
void SelfConfiguringPlugin::setPluginType(const std::string & pluginType) {
    this->pluginType = std::string(pluginType);
}

void SelfConfiguringPlugin::setParams(const std::unordered_map<std::string,std::string> & params) {
    this->params.clear();
    for (auto it: params) {
        this->params.insert(std::make_pair(it.first, it.second));
    }
}

std::string SelfConfiguringPlugin::getPluginType() {
    return pluginType;
}

const std::unordered_map<std::string,std::string> & SelfConfiguringPlugin::getParams() {
    return params;
}
