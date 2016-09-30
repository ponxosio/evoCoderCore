/*
 * CommunicationsInterface.cpp
 *
 *  Created on: 15 de may. de 2016
 *      Author: angel
 */

#include "CommunicationsInterface.h"

CommunicationsInterface *CommunicationsInterface::m_pInstance = NULL;

void CommunicationsInterface::freeCommandInterface() {
	if (m_pInstance) {
		delete m_pInstance;
	}
}

CommunicationsInterface::CommunicationsInterface() {
	this->communicationMap = new std::unordered_map<int, CommandSender*>();
	this->testCommunicationMap = new std::unordered_map<int, CommandSender*>();
	this->testing = false;
	this->lastId = 0;
	this->testLastId = 0;
}

CommunicationsInterface::~CommunicationsInterface() {
	for (auto it = communicationMap->begin(); it != communicationMap->end();
			++it) {
		CommandSender* actual = (*it).second;
		delete actual;
	}
	delete communicationMap;

	for (auto it = testCommunicationMap->begin();
			it != testCommunicationMap->end(); ++it) {
		CommandSender* actual = (*it).second;
		delete actual;
	}
	delete testCommunicationMap;
}

int CommunicationsInterface::addCommandSender(CommandSender* communications) {
	int nextId;
	if (!testing) {
		nextId = lastId;
		communicationMap->insert(std::make_pair(nextId, communications));
		lastId++;
	} else {
		nextId = testLastId;
		testCommunicationMap->insert(std::make_pair(nextId, communications));
		testLastId++;
	}
	return nextId;
}

int CommunicationsInterface::addCommandSenderTestExec(CommandSender * exec, CommandSender * test)
{
	testLastId = std::max(testLastId, lastId);
	lastId = std::max(testLastId, lastId);
	int nextId = lastId;

	communicationMap->insert(std::make_pair(nextId, exec));
	testCommunicationMap->insert(std::make_pair(nextId, test));

	return nextId;
}

CommandSender* CommunicationsInterface::getCommandSender(int communicationId) {
	
	std::unordered_map<int, CommandSender*>* selected;
	if (!testing) {
		selected = communicationMap;
	} else {
		selected = testCommunicationMap;
	}

	CommandSender* finded = NULL;
	typename std::unordered_map<int, CommandSender*>::iterator it = selected->find(communicationId);
	if (it != selected->end()) {
		finded = (*it).second;
	}
	return finded;
}
