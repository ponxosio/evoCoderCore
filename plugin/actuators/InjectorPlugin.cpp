#include "InjectorPlugin.h"

using namespace boost::python;

InjectorPlugin::InjectorPlugin() :
    Injector(),SelfConfiguringPlugin()
{
	this->referenceName = "";
}

InjectorPlugin::InjectorPlugin(int communications, const std::string & pluginType, const std::unordered_map<std::string,std::string> & params) :
    Injector(communications), SelfConfiguringPlugin(pluginType, params)
{
	this->referenceName = "";
}


InjectorPlugin::~InjectorPlugin()
{
	if (!referenceName.empty()) {
		PythonEnvironment::GetInstance()->deleteInstance(referenceName);
	}
}

void InjectorPlugin::injectLiquid(double rate) throw (std::runtime_error)
{
	try {
		if (referenceName.empty()) {
			referenceName = PythonEnvironment::GetInstance()->makeInstance(this->pluginType, this->params);
		}

		CommandSender* com = CommunicationsInterface::GetInstance()->getCommandSender(communications);
		PythonEnvironment::GetInstance()->getVarInstance(referenceName).attr("injectLiquid")(rate, boost::ref(*com));
	}
	catch (std::invalid_argument & e)
	{
		throw (std::runtime_error(e.what()));
	}
	catch (error_already_set) {
		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);

		std::string error = "";
		char* c_str = PyString_AsString(pvalue);
		if (c_str) {
			error = std::string(c_str);
		}
		throw(std::runtime_error("injectLiquid(), Plugin " + pluginType + ": " + "error at python environment " + error));
	}
}

std::string InjectorPlugin::getInstructions() throw (std::runtime_error)
{
	try {
		if (referenceName.empty()) {
			referenceName = PythonEnvironment::GetInstance()->makeInstance(this->pluginType, this->params);
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
	catch (std::invalid_argument & e)
	{
		throw(std::runtime_error("getInstructions(), Plugin " + pluginType + ": " + "internal error" + std::string(e.what())));
	}
}

int InjectorPlugin::getMovementType() throw (std::runtime_error)
{
	try {
		if (referenceName.empty()) {
			referenceName = PythonEnvironment::GetInstance()->makeInstance(this->pluginType, this->params);
		}

		int movementType = extract<int>(PythonEnvironment::GetInstance()->getVarInstance(referenceName).attr("getMovementType")());
		return movementType;
	}
	catch (error_already_set) {
		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);

		std::string error = "";
		char* c_str = PyString_AsString(pvalue);
		if (c_str) {
			error = std::string(c_str);
		}
		throw(std::runtime_error("getMovementType(), Plugin " + pluginType + ": " + "error at python environment " + error));
	}
	catch (std::invalid_argument & e)
	{
		throw(std::runtime_error("getMovementType(), Plugin " + pluginType + ": " + "internal error" + std::string(e.what())));
	}
}
