#pragma once

#define BASE_PLUGIN_FOLDER "pythonPlugins\\basePlugins"

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <memory>

//boost
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/scoped_array.hpp>

//lib
#include <easylogging++.h>

//local
#include "../fluidControl/executable/containers/actuators/communications/CommandSender.h"
#include "../fluidControl/executable/containers/actuators/communications/SerialSender.h"
#include "../fluidControl/executable/containers/actuators/communications/FileSender.h"

#include "../util/AutoEnumerate.h"
#include "../util/Patch.h"
#include "../util/Utils.h"
#include "PluginFileLoader.h"

class PythonEnvironment
{
public:
	static PythonEnvironment* GetInstance() {
		if (!m_pInstance)
			m_pInstance = new PythonEnvironment();
		return m_pInstance;
	}

	static void freeCommandInterface() {
		if (m_pInstance) {
			delete m_pInstance;
		}
	}

	virtual ~PythonEnvironment();

    std::string makeInstance(const std::string & type, const std::unordered_map<std::string,std::string> & params) throw (std::invalid_argument);
	void deleteInstance(const std::string & varName);
	boost::python::api::object getVarInstance(const std::string & varName) throw (std::invalid_argument);

    boost::python::object executeStaticMethod(const std::string & type, const std::string & method) throw (std::invalid_argument);
	
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

private:
	PythonEnvironment();
	PythonEnvironment(const PythonEnvironment& com) {}
	PythonEnvironment& operator=(const PythonEnvironment&) {}

	static PythonEnvironment *m_pInstance;
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

