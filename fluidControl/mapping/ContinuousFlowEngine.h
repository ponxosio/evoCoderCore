#pragma once

#include <unordered_map>
#include <tuple>
#include <memory>

#include "..\executable\ExecutableMachineGraph.h"
#include "..\..\util\Utils.h"

class ContinuousFlowEngine
{
public:
	ContinuousFlowEngine();
	ContinuousFlowEngine(std::shared_ptr<ExecutableMachineGraph> machine);
	virtual ~ContinuousFlowEngine();

	void updateVolumens(long msPassed);

	void setFlowRate(int idSource, int idTarget, double rate);
protected:
	std::unordered_map<std::tuple<int, int>, double, PairIntIntHashFunction> cFlowsMap;
	std::shared_ptr<ExecutableMachineGraph> machine;
};

