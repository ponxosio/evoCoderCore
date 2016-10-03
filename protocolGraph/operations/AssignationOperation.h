/*
 * AsignationOperation.h
 *
 *  Created on: 23 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_ASSIGNATIONOPERATION_H_
#define SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_ASSIGNATIONOPERATION_H_

#pragma warning( disable : 4290 )

#include <string>

//lib
#include <easylogging++.h>

//local
#include "util/Utils.h"
#include "operables/mathematics/VariableEntry.h"
#include "operables/mathematics/MathematicOperable.h"
#include "protocolGraph/OperationNode.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

#include "evocodercore_global.h"

/**
 * Implements a variable assignation, for example "t = 3 + d*2"
 */
class ASSIGNATIONOPERATION_EXPORT AssignationOperation: public OperationNode {
public:
	//methods of node
	AssignationOperation();
	AssignationOperation(const AssignationOperation & node);

	virtual string toText();
	virtual void loadNode(const string & line) throw (invalid_argument);
	//
	/**
	 *
	 * @param idContainer id of the node
	 * @param receiver variable whose value is going to be changed
	 * @param value numeric value to assign to the variable
	 */
	AssignationOperation(int idContainer,
			std::shared_ptr<VariableEntry> receiver,
			std::shared_ptr<MathematicOperable> value);

	virtual ~AssignationOperation();

	virtual void updateReference(const std::string & reference);

	/**
	 * Executes the assignation
	 */
	virtual void execute() throw(std::invalid_argument);

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	/**
	 * variable whose value is going to be changed
	 */
	std::shared_ptr<VariableEntry> receiver;
	/**
	 * numeric value to assign to the variable
	 */
	std::shared_ptr<MathematicOperable> value;
};

template<class Archive>
inline void AssignationOperation::serialize(Archive& ar,
		const std::uint32_t version) {
	if (version <= 1) {
		OperationNode::serialize(ar, version);
		ar(CEREAL_NVP(receiver), CEREAL_NVP(value));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( AssignationOperation, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(AssignationOperation,"AssignationOperation");

#endif /* SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_ASSIGNATIONOPERATION_H_ */
