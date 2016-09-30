/*
 * VariableEntry.cpp
 *
 *  Created on: 15 de mar. de 2016
 *      Author: angel
 */

#include "../mathematics/VariableEntry.h"

using namespace std;

VariableEntry::VariableEntry() {
	this->name = "undefined";
	this->reference = "undefined";
}

VariableEntry::VariableEntry(const string & name){
		this->name = name;
		this->reference = "undefined";
}

void VariableEntry::updateReference(const std::string & reference) {
	this->reference = reference;
	
	shared_ptr<VariableTable> table = getVariableTable();
	if (!table->containsKey(name)) {
		table->setValue(name, 0.0);
	}
}

double VariableEntry::getValue() throw (std::invalid_argument)  {
	return getVariableTable()->getVaue(name);
}

bool VariableEntry::isPhysical() throw (std::invalid_argument)  {
	return getVariableTable()->getPhysical(name);
}

bool VariableEntry::equal(const MathematicOperable* obj) const{
	bool vuelta = false;
	if (Utils::IsType<VariableEntry, MathematicOperable>(obj)) {
		const VariableEntry* cast = dynamic_cast<const VariableEntry*>(obj);
		vuelta = (cast->name == this->name);
	}
	return vuelta;
}

void VariableEntry::setValue(double value) throw (std::invalid_argument)  {
	getVariableTable()->setValue(name, value);
}

void VariableEntry::setPhysical(bool physical) throw (std::invalid_argument)  {
	getVariableTable()->setPhysical(name, physical);
}
