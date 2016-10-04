/*
 * SerialSender.h
 *
 *  Created on: 7 de may. de 2016
 *      Author: angel
 */

#ifndef SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_ACTUATORS_COMMUNICATIONS_SERIALSENDER_H_
#define SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_ACTUATORS_COMMUNICATIONS_SERIALSENDER_H_

#pragma warning( disable : 4290 )

#include <string>
#include <ios>

//Qt
#include <QtSerialPort/QSerialPort>

//lib
#include <easylogging++.h>

//local
#include "CommandSender.h"
#include "util/Patch.h"

 //cereal
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>

#include "evocodercore_global.h"

/**
 * Class that makes the serial communication
 */
class SERIALSENDER_EXPORT SerialSender : public CommandSender {
public:
	/**
	 * @param device name of the port to communicate to (ex. \\\\.\\COM3)
	 *
	 * @param baudRate Specifies the baud at which the communications device operates (9600 default).
	 * @param byteSize Size of a byte (8 default).
	 * @param stopBits Specifies the number of stop bits to be used. This member can be one of the following values:
	 * 			ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS (ONESTOPBIT default)
	 * @param parity Specifies the parity scheme to be used. This member can be one of the following values:
	 * 			EVENPARITY, MARKPARITY, NOPARITY, ODDPARITY (NOPARITY default).
	 */
	SerialSender(std::string device = "undefined",
            qint32 baudRate = QSerialPort::Baud9600,
            QSerialPort::DataBits byteSize = QSerialPort::Data8,
            QSerialPort::StopBits stopBits = QSerialPort::OneStop,
            QSerialPort::Parity parity = QSerialPort::NoParity,
            int msWait = 2000);
	SerialSender(const SerialSender& ss);
	virtual ~SerialSender();

	/**
	 * Send the string through the serial port.
	 * @param str the number of characters send.
	 */
    virtual unsigned long sendString(const char* str);
	/**
	 * receive a String through the serial port. if no information after maxMsWaitingRead millisenconds
	 * a timeout exception is thrown.
	 * @return the received string.
	 */
	virtual std::string receiveString() throw (std::ios_base::failure);

	virtual std::string readUntil(char endCharacter) throw (std::ios_base::failure);

	virtual void disconnect();

	virtual void connect() throw (std::ios_base::failure);

	virtual void synch() throw (std::ios_base::failure);

	virtual CommandSender* clone();

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);

protected:
	std::string device;

    //handler
    QSerialPort* handler;

	//basic settings
	/**
	 * Specifies the baud at which the communications device operates
	 */
    qint32 baudRate;
	/**
	 * Size of a byte
	 */
    QSerialPort::DataBits byteSize;
	/**
	 * Specifies the number of stop bits to be used. This member can be one of the following values:
	 * ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS
	 */
    QSerialPort::StopBits stopBits;
	/**
	 * Specifies the parity scheme to be used. This member can be one of the following values:
	 * EVENPARITY, MARKPARITY, NOPARITY, ODDPARITY
	 */
    QSerialPort::Parity parity;

    int waitMs;
};

template<class Archive>
inline void SerialSender::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		ar(CEREAL_NVP(device), 
			CEREAL_NVP(baudRate), 
			CEREAL_NVP(byteSize),
			CEREAL_NVP(stopBits),
            CEREAL_NVP(parity),
            CEREAL_NVP(waitMs));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION(SerialSender, (int)1);

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(SerialSender, "SerialSender");

#endif /* SRC_FLUIDCONTROL_EXECUTABLE_CONTAINERS_ACTUATORS_COMMUNICATIONS_SERIALSENDER_H_ */
