/*
 * FileSender.cpp
 *
 *  Created on: 15 de may. de 2016
 *      Author: angel
 */

#include "FileSender.h"

FileSender::FileSender() :
	CommandSender()
{
	this->inputName = "undefined";
	this->outputName = "undefined";
}

FileSender::FileSender(const FileSender & fs) :
	CommandSender(fs)
{
	this->inputName = fs.inputName;
	this->outputName = fs.outputName;
}

FileSender::FileSender(const std::string& outputName,
		const std::string& inputName) :
	CommandSender()
{

	this->inputName = inputName;
	this->outputName = outputName;
}

FileSender::~FileSender() {
	disconnect();
}

unsigned long FileSender::sendString(const std::string& str) {
	if (outFile.is_open()) {
		outFile << str << "\n";
	} else {
		throw(std::ios_base::failure("the connection must be opened first"));
	}
	return str.length();
}

std::string FileSender::receiveString() throw (std::ios_base::failure) {
	return readUntil('\n');
}

std::string FileSender::readUntil(char endCharacter)
		throw (std::ios_base::failure) {
	std::string line;
	if (inFile.is_open()) {
		bool finded = false;
		char c;

		do {
			std::string temp;
			inFile.get(c);
			if (inFile.eof()) {
				inFile.clear();
				inFile.seekg(0, inFile.beg);
				finded = true;
			} else {
				temp += c;
				if (c == endCharacter) {
					finded = true;
				} else if (temp.find_first_not_of(
						"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_")
						== std::string::npos) {
					line += c;
				}
			}
		} while (!finded);
	} else {
		throw(std::ios_base::failure("the connection must be opened first"));
	}
	return line;
}

void FileSender::disconnect() {
	if (outFile.is_open()) {
		outFile.close();
	}
	if (inFile.is_open()) {
		inFile.close();
	}
}

void FileSender::connect() throw (std::ios_base::failure) {
	if (!outFile.is_open()) {
		outFile.open(outputName.c_str());
	} else {
		throw(std::ios_base::failure("the file " + outputName + " is already opened"));
	}
	if (!inFile.is_open()) {
		inFile.open(inputName.c_str());
	} else {
		throw(std::ios_base::failure(
				"the file " + inputName + " is already opened"));
	}

}

CommandSender* FileSender::clone() {
	return new FileSender(*this);
}
