#include "ExecutionMachineServer.h"

using namespace std;

ExecutionMachineServer* ExecutionMachineServer::m_pInstance = NULL;

void ExecutionMachineServer::freeCommandInterface()
{
	if (m_pInstance) {
		delete m_pInstance;
	}
}

ExecutionMachineServer::ExecutionMachineServer()
{
	lastSeries = 0;
    machineMap = make_shared<MachineMap>();
}

ExecutionMachineServer::~ExecutionMachineServer()
{
}

std::string ExecutionMachineServer::addNewMachine(const std::string & jsonFilePath)
{
	std::shared_ptr<ExecutableMachineGraph> mPtr (ExecutableMachineGraph::fromJSON(jsonFilePath));

	int exComId = CommunicationsInterface::GetInstance()->addCommandSenderTestExec(mPtr->getExecCommunicationsPrototypeCopy(), mPtr->getTestCommunicationsPrototypeCopy());
	mPtr->updateCommunicationsInterface(exComId);
	mPtr->updateControlActuators();

	lastSeries = series.getNextValue();
	std::string reference = "machine" + patch::to_string(lastSeries);
	machineMap->insert(std::make_pair(reference, make_tuple(exComId, mPtr)));

	return reference;
}

shared_ptr<ExecutableMachineGraph> ExecutionMachineServer::getMachine(const string & machineName) throw (std::invalid_argument)
{
	auto it = machineMap->find(machineName);
	if (it != machineMap->end()) {
		return get<1>(it->second);
	}
	else {
		throw(std::invalid_argument("the reference for the machine " + machineName + " does not exists"));
	}
}

int ExecutionMachineServer::getMachineComId(const string & machineName) throw (std::invalid_argument)
{
	auto it = machineMap->find(machineName);
	if (it != machineMap->end()) {
		return get<0>(it->second);
	}
	else {
		throw(std::invalid_argument("the reference for the machine " + machineName + " does not exists"));
	}
}

vector<shared_ptr<ExecutableMachineGraph>> ExecutionMachineServer::getAvailableMachines()
{
	vector<shared_ptr<ExecutableMachineGraph>> machines;

	for (auto it = machineMap->begin(); it != machineMap->end(); ++it) {
		machines.push_back(get<1>(it->second));
	}

	return machines;
}
