#include "ExecutionServer.h"

ExecutionServer* ExecutionServer::m_pInstance = NULL;

using namespace std;

void ExecutionServer::freeCommandInterface()
{
	if (m_pInstance) {
		delete m_pInstance;
	}
}

ExecutionServer::ExecutionServer()
{
	
}

ExecutionServer::~ExecutionServer()
{
	
}

std::string ExecutionServer::addProtocolOnNewMachine(const std::string & protocolJson, const std::string & machineJson)
{
	ExecutionMachineServer* machineServer = ExecutionMachineServer::GetInstance();
	string machineReference = machineServer->addNewMachine(machineJson);
	
	vector<int> coms;
	shared_ptr<ExecutableMachineGraph> machine = machineServer->getMachine(machineReference);
	coms.push_back(machineServer->getMachineComId(machineReference));

	shared_ptr<ProtocolGraph> protocol(ProtocolGraph::fromJSON(protocolJson));
	shared_ptr<VariableTable> table = make_shared<VariableTable>();
	
	string executionReference = "execution" + patch::to_string(series.getNextValue());
	shared_ptr<Mapping> map = make_shared<Mapping>(machine, protocol->getName() + "_" + machine->getName(), coms);
	shared_ptr<ExecutionEngine> evocoder = make_shared<ExecutionEngine>(protocol, table, map);

	executionMap.insert(make_pair(executionReference, tuple<string,shared_ptr<ExecutionEngine>>(machineReference, evocoder)));
	protocol->updateReference(executionReference);

	return executionReference;
}

std::string ExecutionServer::addProtocolOnNewMachine(std::shared_ptr<ProtocolGraph> protocol, const std::string & machineJson) {
    ExecutionMachineServer* machineServer = ExecutionMachineServer::GetInstance();
    string machineReference = machineServer->addNewMachine(machineJson);

    vector<int> coms;
    shared_ptr<ExecutableMachineGraph> machine = machineServer->getMachine(machineReference);
    coms.push_back(machineServer->getMachineComId(machineReference));

    shared_ptr<VariableTable> table = make_shared<VariableTable>();
    string executionReference = "execution" + patch::to_string(series.getNextValue());
    shared_ptr<Mapping> map = make_shared<Mapping>(machine, protocol->getName() + "_" + machine->getName(), coms);
    shared_ptr<ExecutionEngine> evocoder = make_shared<ExecutionEngine>(protocol, table, map);

    executionMap.insert(make_pair(executionReference, tuple<string,shared_ptr<ExecutionEngine>>(machineReference, evocoder)));
    protocol->updateReference(executionReference);

    return executionReference;
}

std::string ExecutionServer::addProtocolOnExistingMachine(const std::string & protocolJson, const std::string & machineReference) throw (std::invalid_argument)
{
	ExecutionMachineServer* machineServer = ExecutionMachineServer::GetInstance();

	vector<int> coms;
	shared_ptr<ExecutableMachineGraph> machine = machineServer->getMachine(machineReference);
	coms.push_back(machineServer->getMachineComId(machineReference));

	shared_ptr<ProtocolGraph> protocol(ProtocolGraph::fromJSON(protocolJson));
	shared_ptr<VariableTable> table = make_shared<VariableTable>();

	string executionReference = "execution" + patch::to_string(series.getNextValue());
	shared_ptr<Mapping> map = make_shared<Mapping>(machine, protocol->getName() + "_" + machine->getName(), coms);
	shared_ptr<ExecutionEngine> evocoder = make_shared<ExecutionEngine>(protocol, table, map);

	executionMap.insert(make_pair(executionReference, make_tuple(machineReference, evocoder)));
	protocol->updateReference(executionReference);

	return executionReference;
}

std::string ExecutionServer::addProtocolOnExistingMachine(std::shared_ptr<ProtocolGraph> protocol, const std::string & machineReference) throw (std::invalid_argument) {
    ExecutionMachineServer* machineServer = ExecutionMachineServer::GetInstance();

    vector<int> coms;
    shared_ptr<ExecutableMachineGraph> machine = machineServer->getMachine(machineReference);
    coms.push_back(machineServer->getMachineComId(machineReference));

    shared_ptr<VariableTable> table = make_shared<VariableTable>();
    string executionReference = "execution" + patch::to_string(series.getNextValue());
    shared_ptr<Mapping> map = make_shared<Mapping>(machine, protocol->getName() + "_" + machine->getName(), coms);
    shared_ptr<ExecutionEngine> evocoder = make_shared<ExecutionEngine>(protocol, table, map);

    executionMap.insert(make_pair(executionReference, make_tuple(machineReference, evocoder)));
    protocol->updateReference(executionReference);

    return executionReference;
}

void ExecutionServer::exec(const std::string & reference) throw(std::runtime_error)
{
	auto it = executionMap.find(reference);
	if (it != executionMap.end()) {
		try {
			get<1>(it->second)->exec_general();
		}
		catch (std::runtime_error & e) {
			throw(e);
		}
	}
	else {
		throw(runtime_error("unknow key reference: " + reference));
	}
}

void ExecutionServer::test(const std::string & reference) throw(std::runtime_error)
{
	auto it = executionMap.find(reference);
	if (it != executionMap.end()) {
		try {
			get<1>(it->second)->test();
		}
		catch (std::runtime_error & e) {
			throw(e);
		}
	}
	else {
		throw(runtime_error("unknow key reference: " + reference));
	}
}

std::shared_ptr<ExecutionEngine> ExecutionServer::getEvoCoder(const std::string & reference) throw(std::invalid_argument)
{
	auto it = executionMap.find(reference);
	if (it != executionMap.end()) {
		return get<1>(it->second);
	}
	else {
		throw(std::invalid_argument("ExecutionServer::getEvoCoder(), the key " + reference + " does not exists"));
	}
}

std::string ExecutionServer::getMachineReference(const std::string & reference) throw(std::invalid_argument)
{
	auto it = executionMap.find(reference);
	if (it != executionMap.end()) {
		return get<0>(it->second);
	}
	else {
		throw(std::invalid_argument("ExecutionServer::getMachineReference(), the key " + reference + " does not exists"));
	}
}

std::vector<std::tuple<std::string, std::shared_ptr<ExecutionEngine>>> ExecutionServer::getAvailablesExecutions()
{
	std::vector<std::tuple<std::string, std::shared_ptr<ExecutionEngine>>> availables;

	for (auto it = executionMap.begin(); it != executionMap.end(); ++it) {
		availables.push_back(it->second);
	}

	return availables;
}
