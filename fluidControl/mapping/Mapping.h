/*
 * Mapping.h
 *
 *  Created on: 9 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_MAPPING_H_
#define SRC_FLUIDCONTROL_MAPPING_H_

#pragma warning( disable : 4290 )

#include <stdexcept>
#include <sstream>
#include <vector>

//boost
#include <memory>

//lib
#include <easylogging++.h>

//local
#include "graph/Edge.h"
#include "graph/Flow.h"
#include "fluidControl/executable/ExecutableMachineGraph.h"
#include "fluidControl/executable/containers/actuators/communications/CommunicationsInterface.h"
#include "fluidControl/executable/containers/actuators/communications/CommandSender.h"
#include "fluidControl/machineGraph/MachineGraph.h"
#include "MappingEngine.h"
#include "ContinuousFlowEngine.h"
#include "graph/flowgenerator.h"
#include "graph/flowhash.h"

#include "evocodercore_global.h"

/*** Enum for the operation the mapping will do ***/
typedef enum MappingOperation_ {
	sketch,
	exec_ep,
    exec_general,
    analizeFlowsInTime
} MappingOperation;


class MAPPING_EXPORT Mapping {
public:
    typedef Edge EdgeType;

    Mapping() {
        this->sleepMs = 1000;
    }
	Mapping(std::shared_ptr<ExecutableMachineGraph> machine, const std::string & name, const std::vector<int> & communicationInterface);
	virtual ~Mapping();

	//execution
	void setContinuosFlow(int idSource, int idTarget, double rate) throw (std::runtime_error);
	void transfer(int idSource, int idTarget, double volume) throw (std::runtime_error);
	void mix(int source1, int source2, int target, double volume1, double volume2) throw (std::runtime_error);
	void applyLight(int id, double wavelength, double intensity) throw (std::runtime_error);
	void applyTemperature(int id, double degres) throw (std::runtime_error);
	void stir(int id, double intensity) throw (std::runtime_error);
	void loadContainer(int containerID, double volume);

	double getVolume(int id);
	double measureOD(int id) throw (std::runtime_error);

    double timeStept() throw (std::runtime_error);
    void setTimeStep(long sleepMs);

    //miscelaneous
    std::string printMappingTable();

	//operations
	void doMapping() throw (std::invalid_argument);

	bool isSketching();
	void setSketching();

	bool isExec_ep();
	void setExec_ep();

	bool isExec_general();
	void setExec_general();

    bool isAnalizeFlowInTime();
    void setAnalizeFlowInTime();

	//miscellaneous
	void printSketch(const std::string & path);
	void startCommunications();
	void stopCommunications();
    void cleanUsedResources();

	inline void setTimestamp(long actual) {
		this->lastTimestamp = actual;
	}

	inline void settesting(bool testing) {
		this->testing = testing;
	}
protected:
    MappingOperation operation;
	bool testing;
	
	MappingEngine* engine;
    ContinuousFlowEngine* cfEngine;

    FlowGenerator<EdgeType> actualFlowGenerator;
    std::unordered_set<Flow<EdgeType>, FlowHash<EdgeType>> existingFlowsSet;
	
	MachineGraph* sketch;
	std::shared_ptr<ExecutableMachineGraph> machine;
	
	std::vector<int>* communicationsInterfaces;
	long lastTimestamp;
    long sleepMs;

	//SKETCHING
	void sketching_setContinuosFlow(int idSource, int idTarget, double rate);
	void sketching_transfer(int idSource, int idTarget, double volume);
	void sketching_mix(int source1, int source2, int target, double volume1,
			double volume2);
	void sketching_applyLight(int id, double wavelength, double intensity);
	void sketching_applyTemperature(int id, double degrees);
	void sketching_stir(int id);
	void sketching_loadContainer(int containerID, double volume);

	void sketching_measureOD(int id);
	double sketching_timeStep();
    void sketching_setTimeStep(long sleepMs);

	void transformSourceContainer(int idSource, int idTarget, MachineGraph::ContainerNodePtr sourceNode, MovementType desiredType);
	void transformTargetContainer(int idSource, int idTarget, MachineGraph::ContainerNodePtr targetNode);

	//EXEC
	void exec_setContinuosFlow(int idSource, int idTarget, double rate) throw (std::runtime_error);
	void exec_transfer(int idSource, int idTarget, double volume) throw (std::runtime_error);
	void exec_mix(int source1, int source2, int target, double volume1, double volume2) throw (std::runtime_error);
	void exec_applyLight(int id, double wavelength, double intensity) throw (std::runtime_error);
	void exec_applyTemperature(int id, double degres) throw (std::runtime_error);
	void exec_stir(int id, double intensity) throw (std::runtime_error);
	void exec_loadContainer(int containerID, double volume);

	double exec_getVolume(int id);
	double exec_measureOD(int id) throw (std::runtime_error);

	double exec_timeStep();
    void exec_setTimeStep(long sleepMs);

    //ANALIZING FLOWS
    void analizing_setContinuosFlow(int idSource, int idTarget, double rate) throw (std::runtime_error);

    double analizing_timeStep() throw (std::runtime_error);
    void analizing_setTimeStep(long sleepMs);
};

#endif /* SRC_FLUIDCONTROL_MAPPING_H_ */
