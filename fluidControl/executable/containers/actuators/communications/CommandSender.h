/*
 * CommandSender.h
 *
 *  Created on: 10 de may. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_ACTUATORS_COMMUNICATIONS_COMMANDSENDER_H_
#define SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_ACTUATORS_COMMUNICATIONS_COMMANDSENDER_H_

#include <ios>
#include <string>

class CommandSender {
public:
	CommandSender() {}
	CommandSender(const CommandSender & cs) {}
	virtual ~CommandSender(){};

	virtual CommandSender* clone() = 0;
	virtual unsigned long sendString(const std::string & str) = 0;
	virtual std::string receiveString() throw (std::ios_base::failure) = 0;
	virtual std::string readUntil(char endCharacter) throw (std::ios_base::failure) = 0;
	virtual void disconnect() = 0;
	virtual void connect() throw (std::ios_base::failure) = 0;
	virtual void synch() throw (std::ios_base::failure) = 0;
};

#endif /* SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_ACTUATORS_COMMUNICATIONS_COMMANDSENDER_H_ */
