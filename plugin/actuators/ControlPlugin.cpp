#include "ControlPlugin.h"

using namespace boost::python;

ControlPlugin::ControlPlugin() :
    Control(), SelfConfiguringPlugin()
{
	this->referenceName = "";
}

ControlPlugin::ControlPlugin(int communications, const std::string & name, const std::string & pluginType, const std::unordered_map<std::string,std::string> & params) :
    Control(communications), SelfConfiguringPlugin(name, pluginType, params)
{
	this->referenceName = "";
}


ControlPlugin::~ControlPlugin()
{
	if (!referenceName.empty()) {
		PythonEnvironment::GetInstance()->deleteInstance(referenceName);
	}
}

void ControlPlugin::addConnection(int idSource, int idTarget, int pos) throw (std::runtime_error)
{
	try {
		if (referenceName.empty()) {
			referenceName = PythonEnvironment::GetInstance()->makeInstance(this->pluginType, this->params);
		}

		CommandSender* com = CommunicationsInterface::GetInstance()->getCommandSender(communications);
        PythonEnvironment::GetInstance()->getVarInstance(referenceName).attr("addConnection")(idSource, idTarget, pos, boost::ref(*com));

        auto finded = posMap.find(pos);
        if (finded != posMap.end()) {
            std::array<int, 2> contPair = {idSource, idTarget};
            finded->second = contPair;
        } else {
            std::array<int, 2> contPair = {idSource, idTarget};
            posMap.insert(std::make_pair(pos, contPair));
        }
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

void ControlPlugin::setConnection(int idSource, int idTarget) throw (std::runtime_error)
{
	try {
		if (referenceName.empty()) {
			referenceName = PythonEnvironment::GetInstance()->makeInstance(this->pluginType, this->params);
		}

		CommandSender* com = CommunicationsInterface::GetInstance()->getCommandSender(communications);
		PythonEnvironment::GetInstance()->getVarInstance(referenceName).attr("setConnection")(idSource, idTarget, boost::ref(*com));
	}
	catch (error_already_set) {
		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);
		
		std::string error = "";
		char* c_str = PyString_AsString(pvalue);
		if (c_str) {
			error = std::string(c_str);
		}
		throw(std::runtime_error("setConnection(), Plugin " + pluginType + ": " + "error at python environment " + error));
	}
	catch (std::invalid_argument & e)
	{
		throw(std::runtime_error("setConnection(), Plugin " + pluginType + ": " + "internal error" + std::string(e.what())));
	}
}

void ControlPlugin::clearConnections() throw(std::runtime_error)
{
	try {
		if (referenceName.empty()) {
			referenceName = PythonEnvironment::GetInstance()->makeInstance(this->pluginType, this->params);
		}

		PythonEnvironment::GetInstance()->getVarInstance(referenceName).attr("clearConnections")();
	}
	catch (error_already_set) {
		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);

		std::string error = "";
		char* c_str = PyString_AsString(pvalue);
		if (c_str) {
			error = std::string(c_str);
		}
		throw(std::runtime_error("getInstructions(), Plugin " + pluginType + ": " + "error at python environment " + error));
	}
}

std::string ControlPlugin::getInstructions() throw (std::runtime_error)
{
	try {
		if (referenceName.empty()) {
            referenceName = PythonEnvironment::GetInstance()->makeInstance(pluginType, this->params);
		}

		const char* c_str = extract<const char*>(PythonEnvironment::GetInstance()->getVarInstance(referenceName).attr("getInstructions")());
		return std::string(c_str);
	}
	catch (error_already_set) {
		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);

		std::string error = "";
		char* c_str = PyString_AsString(pvalue);
		if (c_str) {
			error = std::string(c_str);
		}
		throw(std::runtime_error("getInstructions(), Plugin " + pluginType + ": " + "error at python environment " + error));
	}
}

std::vector<int> ControlPlugin::getAvailablePos() throw (std::runtime_error) {
    try {
        if (referenceName.empty()) {
            referenceName = PythonEnvironment::GetInstance()->makeInstance(this->pluginType, this->params);
        }

        CommandSender* com = CommunicationsInterface::GetInstance()->getCommandSender(communications);
        boost::python::list pos = extract<boost::python::list>(PythonEnvironment::GetInstance()->getVarInstance(referenceName).attr("getAvailablePos")(boost::ref(*com)));

        vector<int> vectorPos;
        for (int i = 0; i < boost::python::len(pos) ; i++) {
            vectorPos.push_back(extract<int>(pos[i]));
        }
        return vectorPos;
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
        throw(std::runtime_error("getAvailablePos(), " + pluginType + ": " + "error at python environment " + error));
    }
    catch (std::invalid_argument & e)
    {
        throw(std::runtime_error("getAvailablePos(), " + pluginType + ": " + "internal error" + std::string(e.what())));
    }
}

int ControlPlugin::getMaxConnections() throw (std::runtime_error) {
    try {
        if (referenceName.empty()) {
            referenceName = PythonEnvironment::GetInstance()->makeInstance(this->pluginType, this->params);
        }

        int pos = extract<int>(PythonEnvironment::GetInstance()->getVarInstance(referenceName).attr("getMaxConnections")());
        return pos;
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
        throw(std::runtime_error("getAvailablePos(), " + pluginType + ": " + "error at python environment " + error));
    }
    catch (std::invalid_argument & e)
    {
        throw(std::runtime_error("getAvailablePos(), " + pluginType + ": " + "internal error" + std::string(e.what())));
    }
}

int ControlPlugin::getActualPosition() throw (std::runtime_error) {
    try {
        if (referenceName.empty()) {
            referenceName = PythonEnvironment::GetInstance()->makeInstance(this->pluginType, this->params);
        }

        int pos = extract<int>(PythonEnvironment::GetInstance()->getVarInstance(referenceName).attr("getActualPosition")());
        return pos;
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
        throw(std::runtime_error("getAvailablePos(), " + pluginType + ": " + "error at python environment " + error));
    }
    catch (std::invalid_argument & e)
    {
        throw(std::runtime_error("getAvailablePos(), " + pluginType + ": " + "internal error" + std::string(e.what())));
    }
}

void ControlPlugin::reloadConnections() throw (std::runtime_error) {
    clearConnections();
    for (auto it: posMap) {
        int pos = it.first;
        int source = it.second[0];
        int target = it.second[1];

        addConnection(source, target, pos);
    }
}
