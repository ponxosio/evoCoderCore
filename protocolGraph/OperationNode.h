/*
 * OperationNode.h
 *
 *  Created on: 23 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONNODE_H_
#define SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONNODE_H_

#pragma warning( disable : 4290 )

#include <string>
#include <stdexcept>

//local
#include "graph/Node.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

#include "evocodercore_global.h"

/**
 * Interface that implements a node that can be executed
 */
class OPERATIONNODE_EXPORT OperationNode: public Node {
public:
	OperationNode() :
		Node() 
	{
	}
	OperationNode(const OperationNode & obj) :
		Node(obj) 
	{
	}
	OperationNode(int idContainer) :
		Node(idContainer)
	{
	}
	virtual ~OperationNode(){}

	virtual void execute() throw(std::invalid_argument) = 0;

    virtual void updateReference(const std::string & reference) = 0;

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
};

template<class Archive>
inline void OperationNode::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		Node::serialize(ar, version);
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( OperationNode, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(OperationNode,"OperationNode");

#endif /* SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONNODE_H_ */
