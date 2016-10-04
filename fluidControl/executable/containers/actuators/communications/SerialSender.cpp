/*
 * SerialSender.cpp
 *
 *  Created on: 7 de may. de 2016
 *      Author: angel
 */

#include "SerialSender.h"

SerialSender::SerialSender(std::string device,
        qint32 baudRate,
        QSerialPort::DataBits byteSize,
        QSerialPort::StopBits stopBits,
        QSerialPort::Parity parity,
        int msWait) :
	CommandSender()
{
    this->handler = NULL;
    this->device = device;
	this->baudRate = baudRate;
	this->byteSize = byteSize;
	this->stopBits = stopBits;
	this->parity = parity;
    this->waitMs = msWait;
}

SerialSender::SerialSender(const SerialSender & ss) :
	CommandSender(ss)
{
    this->handler = NULL;
    this->device = ss.device;
	this->baudRate = ss.baudRate;
	this->byteSize = ss.byteSize;
	this->stopBits = ss.stopBits;
	this->parity = ss.parity;
    this->waitMs = ss.waitMs;
}

SerialSender::~SerialSender() {
	disconnect();
    if (handler) {
        delete handler;
    }
}

void SerialSender::disconnect() {
    if (handler && handler->isOpen()) {
        handler->close();
    }
}

unsigned long SerialSender::sendString(const std::string& str) {
    if (handler->isOpen()) {
		throw(std::ios_base::failure("connection has not been established yet"));
	}

    return handler->write(str.c_str());
}

std::string SerialSender::receiveString() throw (std::ios_base::failure) {
    if (handler->isOpen()) {
        throw(std::ios_base::failure("connection has not been established yet"));
    }

    std::string strRead;
    int numRead = 0;
    char buffer[50];

    numRead  = handler->read(buffer, 50);
    while(numRead > 0 && handler->waitForReadyRead(waitMs)) {
        strRead = strRead + std::string(buffer, sizeof(buffer));
        numRead  = handler->read(buffer, 50);
    }
    return strRead;
}

void SerialSender::connect() throw (std::ios_base::failure) {
    if (!handler) {
        handler = new QSerialPort(QString::fromStdString(device));
        handler->setBaudRate(baudRate);
        handler->setDataBits(byteSize);
        handler->setParity(parity);
        handler->setStopBits(stopBits);
    }
    if (!handler->isOpen()) {
        if (!handler->open(QSerialPort::ReadWrite)) {
            throw (std::ios_base::failure("imposible to open the port: code error " + patch::to_string(handler->error())));
        }
    }
}

void SerialSender::synch() throw (std::ios_base::failure) {
    Sleep(1500);
}

std::string SerialSender::readUntil(char endCharacter)
        throw (std::ios_base::failure)
{
    if (handler->isOpen()) {
        throw(std::ios_base::failure("connection has not been established yet"));
    }

    std::string read = receiveString();
    size_t pos = read.find_last_of(endCharacter);
    if (pos != std::string::npos) {
        return read.substr(0, pos);
    } else {
        throw(std::ios_base::failure("no end character received"));
    }
}

CommandSender* SerialSender::clone() {
	return new SerialSender(*this);
}
