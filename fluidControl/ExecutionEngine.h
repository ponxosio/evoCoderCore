/*
 * ExecutionEngine.h
 *
 *  Created on: 6 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_EVOCODER_H_
#define SRC_FLUIDCONTROL_EVOCODER_H_

#pragma warning( disable : 4290 )

/***** OPERATION MODES *****/
#define SCHEDULE		0
#define	EXEC_GENERAL	1
#define EXEC_EP			2
#define DESIGN_TEST		3

//includes
#include <algorithm>
#include <string>
#include <stdexcept>
#include <ios>

//data structures
#include <queue>
#include <vector>

//lib
#include <easylogging++.h>

//boost
#include <memory>

//local
#include "util/Utils.h"
#include "protocolGraph/ProtocolGraph.h"
#include "protocolGraph/OperationNode.h"
#include "protocolGraph/operations/LoopNode.h"
#include "protocolGraph/operations/DivergeNode.h"
#include "fluidControl/mapping/Mapping.h"

#include "fluidControl/machineGraph/MachineGraph.h"
#include "fluidControl/executable/ExecutableMachineGraph.h"
#include "fluidControl/executable/containers/actuators/communications/CommunicationsInterface.h"

#include "evocodercore_global.h"

class EXECUTIONENGINE_EXPORT ExecutionEngine {
public:

	//type defs

	typedef queue<ProtocolGraph::ProtocolNodePtr> ProtocolNodeQueue;

	//

	/**
	 *
	 * @param protocol graph with the protocol being executed
	 * @param machine graph with the description of the machine where the protocol will
	 * be executed
	 * @param table table where the values of the variable of the protocol will be stored
	 * @param name name of the portocol
	 */
	ExecutionEngine(std::shared_ptr<ProtocolGraph> protocol,
		std::shared_ptr<VariableTable> table, std::shared_ptr<Mapping> mapping);
	ExecutionEngine(const ExecutionEngine & ec);
	virtual ~ExecutionEngine();

	/**
	 * Tries to execute the protocol on the machine
	 * @return true if all was correct, false otherwise
	 */
	bool exec_general() throw (std::runtime_error);
	bool exec_ep() throw (std::runtime_error);

	bool sketcher();

    void printProtocol(const std::string & path);
	/**
	 * Tries to execute a test of the protocol on the machine
	 * @return true if all was correct, false otherwise
	 */
	bool test() throw (std::runtime_error);

	//getters & setters
	inline std::shared_ptr<VariableTable> getVariableTable()
	{
		return table;
	}

	inline std::shared_ptr<Mapping> getMapping()
	{
		return mapping;
	}
protected:
	/**
	 * protocol graph with the protocol being executed
	 */
	std::shared_ptr<ProtocolGraph> protocol;
	/**
	 * Mapping between the sketch machine and the real one
	 */
	std::shared_ptr<Mapping> mapping;
	/**
	 * table table where the values of the variable of the protocol will be stored
	 */
	std::shared_ptr<VariableTable> table;
	/**
	 * actual node of the protocol
	 */
	ProtocolGraph::ProtocolNodePtr actual;


	//METHODS
	void initilizeTime();
	void addAvailableEdges(ProtocolNodeQueue & nodes);

	//exec methods
	bool exec();

};

#endif /* SRC_FLUIDCONTROL_EVOCODER_H_ */
