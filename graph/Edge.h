/*
 * Edge.h
 *
 *  Created on: 7 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_GRAPH_EDGE_H_
#define SRC_GRAPH_EDGE_H_

#include <string>
#include  <stdexcept>

//boost
#include <boost/algorithm/string/trim.hpp>

//local
#include "../util/Patch.h"

//cereal
#include <cereal\cereal.hpp>

/**
 * Interface with the minimum operations that all directed Edges must have
 */
class Edge {

public:
	//All derived clases must implement this two Constructor!!
	Edge() {
		this->idSource = 0;
		this->idTarget = 0;
	}

	Edge(const Edge & edge) {
		this->idSource = edge.idSource;
		this->idTarget = edge.idTarget;
	}
	//
	Edge(int idSource, int idTarget) {
		this->idSource = idSource;
		this->idTarget = idTarget;
	}

	virtual ~Edge(){}
	
	/**
	 * Returns the source node of the edge
	 * @return the ID of the source node
	 */
	inline int getIdSource() {
		return idSource;
	}
	/**
	 * Returns the target node of the edge
	 * @return the ID of the target node
	 */
	inline int getIdTarget() {
		return idTarget;
	}
	
	/**
	 * Compares two edges
	 *
	 * @param e other edge to compare
	 * @return true if the edges are equal, false otherwise
	 */
	inline virtual bool equals (const Edge& e) {
		return ((e.idSource == this->idSource) && (e.idTarget == this->idTarget));
	}

	/**
	 * Returns a string with all the information of the edge, must be with a format that loadEdge can manage.
	 *
	 * @return a string with all the necessary information to store this edge
	 */
	inline virtual std::string toText() {
		return patch::to_string(idSource) + "->" + patch::to_string(idTarget) + ";";
	}
	/**
	 * Loads all edge information, changing the object's internal attributes. throw invalid_argument if the line has
	 * an incorrect format.
	 *
	 * @param line text with the information of the edge, must be the compatible with the output of the above
	 * toText function
	 */
	inline virtual void loadEdge(const std::string & line) throw(std::invalid_argument) {
		std::string errMessage = "";
		std::size_t posArrow = line.find("->");
		std::size_t posEnd = line.find(";");

		if ((posArrow != std::string::npos) && (posEnd != std::string::npos)) {
			std::string chunkSource = line.substr(0, posArrow);
			std::string chunkTarget = line.substr(posArrow + 2, posEnd - (posArrow + 2));

			boost::algorithm::trim(chunkSource);
			boost::algorithm::trim(chunkTarget);

			int idSource;
			int idTarget;
			const char * cSource = chunkSource.c_str();
			const char * cTarget = chunkTarget.c_str();
			if ((patch::stol(idSource, cSource)) && (patch::stol(idTarget, cTarget))) {
				this->idSource = idSource;
				this->idTarget = idTarget;
			} else {
				errMessage = "\"" + chunkSource + "\" or \"" + chunkTarget + ", are not integer numbers.";
			}
		} else {
			errMessage = "wrong format  \"idSource -> idTarget ;\"";
		}

		if (!errMessage.empty()) {
			throw(std::invalid_argument(errMessage));
		}
	}

	/**
	 * return if this line has the format for an edge, this method must changed depending the format used for the edge
	 * @param line text that is going to be checked
	 * @return true if the format is for an edge, false otherwise
	 */
	inline virtual bool isEdge (const std::string & line) {
		return (line.find("->") != std::string::npos);
	}

	//SERIALIZATIoN
	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);

protected:
	int idSource;
	int idTarget;

};

template<class Archive>
inline void Edge::serialize(Archive& ar, const std::uint32_t version) {
	if (version <= 1) {
		ar(CEREAL_NVP(idSource), CEREAL_NVP(idTarget));
	}
}
// Associate some type with a version number
CEREAL_CLASS_VERSION(Edge, (int)1);


#endif /* SRC_GRAPH_EDGE_H_ */
