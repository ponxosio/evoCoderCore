#pragma once

#include <string>
#include <stdexcept>
#include <unordered_map>

//lib
#include <easylogging++.h>

//local
#include "ExecutionMachineServer.h"
#include "fluidControl\ExecutionEngine.h"
#include "fluidControl\executable\ExecutableMachineGraph.h"
#include "fluidControl\executable\containers\actuators\communications\CommunicationsInterface.h"
#include "fluidControl\executable\containers\actuators\communications\CommandSender.h"
#include "operables\VariableTable.h"
#include "plugin\PythonEnvironment.h"
#include "protocolGraph\ProtocolGraph.h"
#include "util\AutoEnumerate.h"
#include "util\Patch.h"
#include "util\Utils.h"

#include "evocodercore_global.h"

class EXECUTIONSERVER_EXPORT ExecutionServer
{
public:
	static ExecutionServer* GetInstance() {
		if (!m_pInstance)
			m_pInstance = new ExecutionServer();
		return m_pInstance;
	}
	static void freeCommandInterface();

	~ExecutionServer();

	//protocols operation
	std::string addProtocolOnNewMachine(const std::string & protocolJson, const std::string & machineJson);
	std::string addProtocolOnExistingMachine(const std::string & protocolJson, const std::string & machineReference) throw (std::invalid_argument);

	void exec(const std::string & reference) throw(std::runtime_error);
	void test(const std::string& reference) throw(std::runtime_error);

	//getters
	std::shared_ptr<ExecutionEngine> getEvoCoder(const std::string & reference) throw (std::invalid_argument);
	std::string getMachineReference(const std::string& reference) throw (std::invalid_argument);

	std::vector<std::tuple<std::string, std::shared_ptr<ExecutionEngine>>> getAvailablesExecutions();

private:
	static ExecutionServer *m_pInstance;

	AutoEnumerate series;
	std::unordered_map<std::string, std::tuple<std::string, std::shared_ptr<ExecutionEngine>>> executionMap;

	ExecutionServer();
    ExecutionServer(const ExecutionServer&) {}
    ExecutionServer& operator=(const ExecutionServer& com) {return ExecutionServer(com);}
};

