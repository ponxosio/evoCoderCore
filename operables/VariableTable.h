/*
 * VariableTable.h
 *
 *  Created on: 15 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_OPERABLES_VARIABLETABLE_H_
#define SRC_OPERABLES_VARIABLETABLE_H_

#include <string>
#include  <stdexcept>
#include <memory>

// data structures
#include <tuple>
#include <unordered_map>

//lib
#include <cereal/cereal.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>


/**
 * Class that implements an associative table where the value of the declared variables in a protocol will be stored.
 *
 * Also each variable has a flag that tells if the variable is used to store a physical property of the machine.
 *
 * Also has methods to serialize and deserialize the object in order to store it at the disk.
 */
class VariableTable {
public:
	VariableTable();
	virtual ~VariableTable();

	/**
	 * Returns the value of the table entry, throws an invalid argument exception if the entry does not exists
	 * @param name key of the entry
	 * @return the value of that entry
	 */
	double getVaue(const std::string & name) throw (std::invalid_argument);
	/**
	 * Returns if a variable stores physical data.
	 * @param name of the variable
	 * @return true if te variable stores physical data, false otherwise
	 */
	bool getPhysical(const std::string & name) throw (std::invalid_argument);
	/**
	 * Change the value of a given entry of the table, creating it if the name does not exists
	 * @param name key of the entry to be changed
	 * @param value new value of the entry
	 */
	void setValue(const std::string & name, double value);
	/**
	 * Set the physical operation of the variable
	 * @param name of the variable to change
	 * @param physical flag that indicates that a variable stores a physical property of the machine
	 */
	void setPhysical(const std::string & name, bool physical);
	/**
	 * Clears all variables at the table
	 */
	inline void clear() {table.clear();}

	inline bool containsKey(const std::string & key)
	{
		return (table.find(key) != table.end());
	}

	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);

protected:
	std::unordered_map<std::string, std::tuple<double,bool>> table;
};

template<class Archive>
inline void VariableTable::serialize(Archive& ar, std::uint32_t const version) {
	if (version == 1) {
		ar(CEREAL_NVP(table));
	}
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( VariableTable, 1 );

#endif /* SRC_OPERABLES_VARIABLETABLE_H_ */
