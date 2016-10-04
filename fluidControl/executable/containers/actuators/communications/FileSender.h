/*
 * FileSender.h
 *
 *  Created on: 15 de may. de 2016
 *      Author: angel
 */

#ifndef FLUIDCONTROL_EXECUTABLE_CONTAINERS_ACTUATORS_COMMUNICATIONS_FILESENDER_H_
#define FLUIDCONTROL_EXECUTABLE_CONTAINERS_ACTUATORS_COMMUNICATIONS_FILESENDER_H_

#pragma warning( disable : 4290 )

#include <iostream>
#include <fstream>
#include <ios>
#include <cfloat>
#include <string>

#include "CommandSender.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>

#include "evocodercore_global.h"

class FILESENDER_EXPORT FileSender: public CommandSender {
public:
	FileSender();
	FileSender(const FileSender & fs);
	FileSender(const std::string & outputName, const std::string & inputName);
	virtual ~FileSender();

	virtual CommandSender* clone();
    virtual unsigned long sendString(const char* str);
	virtual std::string receiveString() throw (std::ios_base::failure);
	virtual std::string readUntil(char endCharacter)
			throw (std::ios_base::failure);
	virtual void disconnect();
	virtual void connect() throw (std::ios_base::failure);
    inline virtual void synch() throw (std::ios_base::failure){}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
protected:
	std::string outputName;
	std::string inputName;

	std::ofstream outFile;
	std::ifstream inFile;
};

template<class Archive>
inline void FileSender::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		ar(CEREAL_NVP(outputName), CEREAL_NVP(inputName));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION(FileSender, (int)1);

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(FileSender, "FileSender");

#endif /* FLUIDCONTROL_EXECUTABLE_CONTAINERS_ACTUATORS_COMMUNICATIONS_FILESENDER_H_ */
