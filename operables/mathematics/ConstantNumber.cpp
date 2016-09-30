/*
 * ConstantNumber.cpp
 *
 *  Created on: 15 de mar. de 2016
 *      Author: angel
 */

#include "../mathematics/ConstantNumber.h"

ConstantNumber::ConstantNumber() {
	this->value = -1.0;
}
ConstantNumber::ConstantNumber(int value) {
	this->value = value;
}

ConstantNumber::ConstantNumber(double value) {
	this->value = value;
}

double ConstantNumber::getValue() {
	return value;
}

bool ConstantNumber::equal(const MathematicOperable* obj) const {
	bool vuelta = false;
	if (Utils::IsType<ConstantNumber, MathematicOperable>(obj)) {
		const ConstantNumber* cast = dynamic_cast<const ConstantNumber*>(obj);
		vuelta = (cast->value == this->value);
	}
	return vuelta;
}
