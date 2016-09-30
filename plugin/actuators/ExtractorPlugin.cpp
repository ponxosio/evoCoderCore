#include "ExtractorPlugin.h"

using namespace boost::python;

ExtractorPlugin::ExtractorPlugin() :
    Extractor(), SelfConfiguringPlugin()
{
	this->referenceName = "";
}

ExtractorPlugin::ExtractorPlugin(int communications, const std::string & pluginType, const std::unordered_map<std::string,std::string> & params) :
    Extractor(communications), SelfConfiguringPlugin(pluginType, params)
{
	this->referenceName = "";
}

ExtractorPlugin::~ExtractorPlugin()
{
	if (!referenceName.empty()) {
		PythonEnvironment::GetInstance()->deleteInstance(referenceName);
	}
}

void ExtractorPlugin::extractLiquid(double rate) throw (std::runtime_error)
{
	try {
		if (referenceName.empty()) {
			referenceName = PythonEnvironment::GetInstance()->makeInstance(this->pluginType, this->params);
		}

		CommandSender* com = CommunicationsInterface::GetInstance()->getCommandSender(communications);
		PythonEnvironment::GetInstance()->getVarInstance(referenceName).attr("extractLiquid")(rate, boost::ref(*com));
	}
	catch (error_already_set) {
		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);

		std::string error = "";
		char* c_str = PyString_AsString(pvalue);
		if (c_str) {
			error = std::string(c_str);
		}
		throw(std::runtime_error("extractLiquid(),Plugin " + pluginType + ": " + "error at python environment " + error));
	}
	catch (std::invalid_argument & e)
	{
		throw(std::runtime_error("extractLiquid(),Plugin " + pluginType + ": " + "internal error" + std::string(e.what())));
	}

}

std::string ExtractorPlugin::getInstructions() throw (std::runtime_error) {
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
		throw(std::runtime_error("getInstructions(),Plugin " + pluginType + ": " + "error at python environment " + error));
	}
	catch (std::invalid_argument & e)
	{
		throw(std::runtime_error("getInstructions(),Plugin " + pluginType + ": " + "internal error" + std::string(e.what())));
	}
}

int ExtractorPlugin::getMovementType() throw (std::runtime_error)
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
