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

//static
std::vector<std::pair<std::string,std::string>> SelfConfiguringPlugin::getParamsType(const std::string & pluginType) throw (std::runtime_error) {
    try {
        std::vector<std::pair<std::string,std::string>> params;
        dict dictionary = extract<dict>(PythonEnvironment::GetInstance()->executeStaticMethod(pluginType, "getParamsType"));

        boost::python::list lista = dictionary.keys();
        int size = len(lista);

        for (int i = 0; i < size; i++) {
            boost::python::extract<boost::python::object> key_extract(lista[i]);
            if (key_extract.check()) {
                boost::python::object u = key_extract();
                const char* cKey = extract<const char*>(str(u).encode("utf-8"));
                std::string keyStr(cKey);

                boost::python::extract<boost::python::object> value_extract(dictionary[keyStr]);
                if (value_extract.check()) {
                    u = value_extract();
                    const char* cValue = extract<const char*>(str(u).encode("utf-8"));
                    std::string valueStr(cValue);

                    params.push_back(make_pair(keyStr, valueStr));
                } else {
                    LOG(ERROR) << "bad value at position " << i;
                }
            } else {
                LOG(ERROR) << "bad value at position " << i;
            }
        }
        return params;
    }
    catch (error_already_set)
    {
        PyObject *ptype, *pvalue, *ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);

        std::string error = "";
        char* c_str = PyString_AsString(pvalue);
        if (c_str) {
            error = std::string(c_str);
        }
        throw(std::runtime_error("addConnection(), " + pluginType + ": " + "error at python environment " + error));
    }
    catch (std::invalid_argument & e)
    {
        throw(std::runtime_error("addConnection(), " + pluginType + ": " + "internal error" + std::string(e.what())));
    }
}
