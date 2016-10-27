#pragma once

#pragma data_seg

#pragma warning( disable : 4290 )

#define BASE_PLUGIN_FOLDER "pythonPlugins\\basePlugins"

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <stdexcept>

//boost
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/scoped_array.hpp>

//lib
#include <easylogging++.h>

//local
#include "fluidControl/executable/containers/actuators/communications/CommandSender.h"
#include "fluidControl/executable/containers/actuators/communications/SerialSender.h"
#include "fluidControl/executable/containers/actuators/communications/FileSender.h"

#include "util/AutoEnumerate.h"
#include "util/Patch.h"
#include "util/Utils.h"
#include "PluginFileLoader.h"

#include "evocodercore_global.h"

class PYTHONENVIRONMENT_EXPORT PythonEnvironment
{
public:
    static PythonEnvironment* GetInstance();
    static void freeCommandInterface();

	virtual ~PythonEnvironment();

    std::string makeInstance(const std::string & type, const std::unordered_map<std::string,std::string> & params) throw (std::invalid_argument);
	void deleteInstance(const std::string & varName);
	boost::python::api::object getVarInstance(const std::string & varName) throw (std::invalid_argument);

    boost::python::object executeStaticMethod(const std::string & type, const std::string & method) throw (std::invalid_argument);
    std::vector<std::pair<std::string,std::string>> getParamsType(const std::string & pluginType) throw (std::runtime_error);
	
	void initEnvironment();
	void finishEnvironment();

protected:
	std::unordered_map<std::string, boost::python::api::object> loadedClasses;
	std::unordered_set<std::string> loadedFiles;
	std::unordered_set<std::string> availableInstances;
	boost::python::api::object main_namespace;
	AutoEnumerate autoEnum;

	void addImportPath(const std::string & path);
	std::string getPythonClassNAme(const std::string & path) throw (std::invalid_argument);
	std::string getDirectoryPath(const std::string & path) throw (std::invalid_argument);
    bool isVirtualType(const std::string & pluginType);
    void getParamsVirtualType(const std::string & pluginType, std::vector<std::pair<std::string,std::string>> & paramsType);

    static PythonEnvironment* m_pInstance;

private:
	PythonEnvironment();
    PythonEnvironment(const PythonEnvironment&) {}
    PythonEnvironment& operator=(const PythonEnvironment& py) {return PythonEnvironment(py);}


};

struct CommandSenderWrap : CommandSender, boost::python::wrapper<CommandSender>
{
	unsigned long sendString(const std::string & str) {
		return this->get_override("sendString")(str);
	}
	std::string receiveString() {
		return this->get_override("receiveString")();
	}
	std::string readUntil(char endCharacter) {
		return this->get_override("readUntil")();
	}
	void synch() {
		this->get_override("synch")();
	}

	void disconnect() {
		this->get_override("disconnect")();
	}
	
	void connect() {
		this->get_override("connect")();
	}
};

