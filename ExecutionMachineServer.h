#pragma once

#include <string>
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <tuple>

//lib
#include <easylogging++.h>

//local
#include "fluidControl\executable\ExecutableMachineGraph.h"
#include "fluidControl\executable\containers\actuators\communications\CommunicationsInterface.h"
#include "fluidControl\executable\containers\actuators\communications\CommandSender.h"
#include "util\AutoEnumerate.h"
#include "util\Patch.h"

class ExecutionMachineServer
{
public:

	typedef std::unordered_map<std::string, std::tuple<int, std::shared_ptr<ExecutableMachineGraph>>> MachineMap;

	static ExecutionMachineServer* GetInstance() {
		if (!m_pInstance)
			m_pInstance = new ExecutionMachineServer();
		return m_pInstance;
	}
	static void freeCommandInterface();

	~ExecutionMachineServer();

	std::string addNewMachine(const std::string & jsonFilePath);
	std::shared_ptr<ExecutableMachineGraph> getMachine(const std::string & machineName) throw (std::invalid_argument);
	int getMachineComId(const std::string & name);
	std::vector<std::shared_ptr<ExecutableMachineGraph>> getAvailableMachines();

	//getter & setter
	inline std::shared_ptr<MachineMap> getMachineMap()
	{
		return machineMap;
	}

private:
	static ExecutionMachineServer *m_pInstance;

	std::shared_ptr<MachineMap> machineMap;
	AutoEnumerate series;
	int lastSeries;

	ExecutionMachineServer();
	ExecutionMachineServer(const CommunicationsInterface& com) {}
	ExecutionMachineServer& operator=(const CommunicationsInterface&) {}
};

