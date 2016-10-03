/*
 * DivergeNode.h
 *
 *  Created on: 11 de abr. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_DIVERGENODE_H_
#define SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_DIVERGENODE_H_

//types strings
#define DIVERGE_STRING "if"

#include <string>

//boost
#include <memory>

//lib
#include <easylogging++.h>
//local
#include "../../util/Utils.h"
#include "../../operables/comparison/ComparisonOperable.h"
#include "../OperationNode.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

#include "evocodercore_global.h"

class /*DIVERGENODE_EXPORT*/ DivergeNode: public OperationNode {
public:
	//Methods for Node
	DivergeNode();
	DivergeNode(const DivergeNode & node);

	virtual string toText();
	virtual void loadNode(const string & line) throw (invalid_argument);
	//
	DivergeNode(int containerId,
			std::shared_ptr<ComparisonOperable> conditionIN);
	virtual ~DivergeNode();

	virtual void updateReference(const std::string & reference);

	virtual void execute();

	//GETTERS & SETTERS
	const std::shared_ptr<ComparisonOperable>& getConditionIN() const {
		return conditionIN;
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	std::shared_ptr<ComparisonOperable> conditionIN;
};

template<class Archive>
inline void DivergeNode::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		OperationNode::serialize(ar, version);
		ar(CEREAL_NVP(conditionIN));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( DivergeNode, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(DivergeNode,"DivergeNode");
#endif /* SRC_FLUIDCONTROL_PROTOCOLGRAPH_OPERATIONS_DIVERGENODE_H_ */
