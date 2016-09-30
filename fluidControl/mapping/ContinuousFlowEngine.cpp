#include "ContinuousFlowEngine.h"



ContinuousFlowEngine::ContinuousFlowEngine()
{
	this->machine = std::make_shared<ExecutableMachineGraph>();
}


ContinuousFlowEngine::ContinuousFlowEngine(std::shared_ptr<ExecutableMachineGraph> machine)
{
	this->machine = machine;
}

ContinuousFlowEngine::~ContinuousFlowEngine()
{
}

void ContinuousFlowEngine::updateVolumens(long msPassed)
{
	for (auto it = cFlowsMap.begin(); it != cFlowsMap.end(); ++it) {
		std::tuple<int, int> actualFlow = it->first;
		double rate = it->second;

		double mlExtracted = rate * msPassed;
		machine->substractVolume(std::get<0>(actualFlow), mlExtracted);
		machine->addVolume(std::get<1>(actualFlow), mlExtracted);
	}
}

void ContinuousFlowEngine::setFlowRate(int idSource, int idTarget, double rate)
{
	auto actual = cFlowsMap.find(std::tuple<int, int>(idSource, idTarget));
	if (actual != cFlowsMap.end()) {
		actual->second = rate;
	}
	else {
		std::tuple<int, int> newKey(idSource, idTarget);
		cFlowsMap.insert(std::make_pair(newKey, rate));
	}
}
