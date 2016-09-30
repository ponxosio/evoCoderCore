/*
 * Instructable.h
 *
 *  Created on: 3 de may. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_EXECUTABLEMACHINEGRAPH_CONTAINERS_ACTUATORSINTERFACES_INSTRUCTABLE_H_
#define SRC_FLUIDCONTROL_EXECUTABLEMACHINEGRAPH_CONTAINERS_ACTUATORSINTERFACES_INSTRUCTABLE_H_

#include <string>

class Instructable {
public:
	Instructable(){}
	virtual ~Instructable(){}
	virtual std::string getInstructions() = 0;
};



#endif /* SRC_FLUIDCONTROL_EXECUTABLEMACHINEGRAPH_CONTAINERS_ACTUATORSINTERFACES_INSTRUCTABLE_H_ */
