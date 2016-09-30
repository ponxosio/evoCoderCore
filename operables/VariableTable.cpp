/*
 * VariableTable.cpp
 *
 *  Created on: 15 de mar. de 2016
 *      Author: angel
 */

#include "VariableTable.h"

using namespace std;

VariableTable::VariableTable() {
	table = unordered_map<string,  std::tuple<double,bool>>();
}

VariableTable::~VariableTable() {
}

double VariableTable::getVaue(const std::string& name)
		throw (std::invalid_argument) {

	double vuelta = 0.0;
	auto entry = table.find(name);
	if (entry != table.end()) {
		vuelta = std::get<0>(entry->second);
	} else {
		throw(invalid_argument("the key:\"" + name + "\" does not exits"));
	}
	return vuelta;
}

bool VariableTable::getPhysical(const std::string& name)
		throw (std::invalid_argument) {

	bool vuelta = false;
	auto entry = table.find(name);
	if (entry != table.end()) {
		vuelta = std::get<1>(entry->second);
	} else {
		throw(invalid_argument("the key:\"" + name + "\" does not exits"));
	}
	return vuelta;
}

void VariableTable::setValue(const string& name, double value) {
	auto entry = table.find(name);
	if (entry != table.end()) {
		get<0>(entry->second) = value;
	} else {
		table.insert(make_pair(name, make_tuple(value,false)));
	}
}

void VariableTable::setPhysical(const string& name, bool physical) {
	auto entry = table.find(name);
	if (entry != table.end()) {
		get<1>(entry->second) = physical;
	} else {
		table.insert(make_pair(name, make_tuple(0.0, physical)));
	}
}
