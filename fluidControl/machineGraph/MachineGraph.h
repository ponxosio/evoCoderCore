/*
 * ProtocolGraph.h
 *
 *  Created on: 9 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_MACHINEGRAPH_MACHINEGRAPH_H_
#define SRC_FLUIDCONTROL_MACHINEGRAPH_MACHINEGRAPH_H_

#include <string>

//data structed
#include <unordered_map>

//local
#include "../../graph/Graph.h"
#include "../../graph/Edge.h"
#include "ContainerNode.h"
#include "ContainerNodeType.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>


class MachineGraph {
public:
	
	//Type defs
	typedef Graph<ContainerNode,Edge>::NodeTypePtr ContainerNodePtr;
	typedef Graph<ContainerNode, Edge>::EdgeTypePtr ContainerEdgePtr;

	typedef Graph<ContainerNode, Edge>::NodeVector ContainerNodeVector;
	typedef Graph<ContainerNode, Edge>::EdgeVector ContainerEdgeVector;

	typedef Graph<ContainerNode, Edge>::NodeVectorPtr ContainerNodeVectorPtr;
	typedef Graph<ContainerNode, Edge>::EdgeVectorPtr ContainerEdgeVectorPtr;

	typedef Graph<ContainerNode, Edge>::SubGraphPtr ContainerSubGraphPtr;

	typedef std::shared_ptr<Graph<ContainerNode, Edge>> ContainerNodeGraphPtr;
	//

	//static
	static void toJSON(const std::string & path, const MachineGraph & machine);
	static MachineGraph* fromJSON(const std::string & path);
	//
	
	MachineGraph();
	MachineGraph(const MachineGraph & machine);
	MachineGraph(std::string name);
	virtual ~MachineGraph();

	bool addContainer(int idContainer, std::shared_ptr<ContainerNodeType> type, float capacity);
	bool connectContainer(int idSource, int idTarget);

	bool changeContainerType(int idContainer, ContainerType type);
	bool changeMovementType(int idContainer, MovementType type);
	bool addAddon(int idContainer, AddOnsType type);
	bool removeAddon(int idContainer, AddOnsType type);

	bool areConected(int idSource, int idTarget);
	bool hasConections(int idContainer);

	void printMachine(const std::string & path);

	/**
	 * return if a certain container already exists in the machine
	 * @param idContainer ID of the container
	 * @return true if the container is already in the machine, false otherwise
	 */
	inline bool existsContainer(int idContainer) {
		return (machine->getNode(idContainer) != NULL);
	}

	inline typename ContainerNodePtr getContainer(int idContainer) {
		return (machine->getNode(idContainer));
	}

	inline typename ContainerSubGraphPtr getSubgraphs() {
		return machine->getSubGraphs();
	}

	inline ContainerNodeGraphPtr getGraph() {
		return machine;
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);

protected:
	std::string name;
	ContainerNodeGraphPtr machine;

	ContainerNodePtr createContainerNode(int idContainer, std::shared_ptr<ContainerNodeType> type,
		float capacity);
	ContainerEdgePtr createContainerEdgePtr(int idSource, int idTarget);

	ContainerNodeGraphPtr makeGraph();
};

template<class Archive>
inline void MachineGraph::serialize(Archive& ar,
	const std::uint32_t version) {
	if (version <= 1) {
		ar(CEREAL_NVP(name), CEREAL_NVP(machine));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION(MachineGraph, (int)1);

#endif /* SRC_FLUIDCONTROL_MACHINEGRAPH_MACHINEGRAPH_H_ */
