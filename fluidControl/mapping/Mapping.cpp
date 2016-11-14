/*
 * Mapping.cpp
 *
 *  Created on: 9 de mar. de 2016
 *      Author: angel
 */

#include "Mapping.h"

Mapping::Mapping(std::shared_ptr<ExecutableMachineGraph> machine, const string & name, const std::vector<int> & communicationInterface) {
	this->machine = machine;
	this->sketch = new MachineGraph(name);
	this->operation = mapping::sketch;
	this->engine = new MappingEngine(this->sketch, machine);
	this->cfEngine = new ContinuousFlowEngine(machine);
	this->lastTimestamp = -1;
	this->testing = false;

	this->communicationsInterfaces = new std::vector<int>();
	for (auto it = communicationInterface.begin(); it != communicationInterface.end(); ++it) {
		this->communicationsInterfaces->push_back(*it);
	}

}

Mapping::~Mapping() {
	delete sketch;
	delete engine;
	delete cfEngine;
	delete communicationsInterfaces;
}

//OPERATIONS
bool Mapping::isSketching() {
	return (operation == mapping::sketch);
}

void Mapping::setSketching() {
	operation = mapping::sketch;
}

bool Mapping::isExec_ep() {
	return (operation == mapping::exec_ep);
}

void Mapping::setExec_ep() {
	operation = mapping::exec_ep;
}

bool Mapping::isExec_general() {
	return (operation == mapping::exec_general);
}

void Mapping::setExec_general() {
	operation = mapping::exec_general;
}

void Mapping::doMapping() throw (std::invalid_argument) {
	if (!engine->startMapping()) {
		throw(std::invalid_argument("mapping cannot be done"));
	}
}

//EXECUTIoN
void Mapping::setContinuosFlow(int idSource, int idTarget, double rate) throw (std::runtime_error) {

	switch (operation) {
	case mapping::sketch:
		sketching_setContinuosFlow(idSource, idTarget, rate);
		break;
	case mapping::exec_general:
		exec_setContinuosFlow(idSource, idTarget, rate);
		break;
	case mapping::exec_ep:
		exec_setContinuosFlow(idSource, idTarget, rate);
		break;
	default:
		break;
	}
}

void Mapping::transfer(int idSource, int idTarget, double volume) throw (std::runtime_error) {

	switch (operation) {
	case mapping::sketch:
		sketching_transfer(idSource, idTarget, volume);
		break;
	case mapping::exec_general:
		exec_transfer(idSource, idTarget, volume);
		break;
	case mapping::exec_ep:
		exec_transfer(idSource, idTarget, volume);
		break;
	default:
		break;
	}
}

void Mapping::mix(int source1, int source2, int target, double volume1, double volume2) throw (std::runtime_error) {
	switch (operation) {
	case mapping::sketch:
		sketching_mix(source1, source2, target, volume1, volume2);
		break;
	case mapping::exec_general:
		exec_mix(source1, source2, target, volume1, volume2);
		break;
	case mapping::exec_ep:
		exec_mix(source1, source2, target, volume1, volume2);
		break;
	default:
		break;
	}
}

void Mapping::applyLight(int id, double wavelength, double intensity) throw (std::runtime_error) {
	switch (operation) {
	case mapping::sketch:
		sketching_applyLight(id, wavelength, intensity);
		break;
	case mapping::exec_general:
		exec_applyLight(id, wavelength, intensity);
		break;
	case mapping::exec_ep:
		exec_applyLight(id, wavelength, intensity);
		break;
	default:
		break;
	}
}

void Mapping::applyTemperature(int id, double degrees) throw (std::runtime_error) {
	switch (operation) {
	case mapping::sketch:
		sketching_applyTemperature(id, degrees);
		break;
	case mapping::exec_general:
		exec_applyTemperature(id, degrees);
		break;
	case mapping::exec_ep:
		exec_applyTemperature(id, degrees);
		break;
	default:
		break;
	}
}

void Mapping::stir(int id, double intensity) throw (std::runtime_error) {
	switch (operation) {
	case mapping::sketch:
		sketching_stir(id);
		break;
	case mapping::exec_general:
		exec_stir(id, intensity);
		break;
	case mapping::exec_ep:
		exec_stir(id, intensity);
		break;
	default:
		break;
	}
}

double Mapping::getVolume(int id) {
	double vuelta = -1.0;

	switch (operation) {
	case mapping::sketch:
		break;
	case mapping::exec_general:
		vuelta = exec_getVolume(id);
		break;
	case mapping::exec_ep:
		vuelta = exec_getVolume(id);
		break;
	default:
		break;
	}
	return vuelta;
}

double Mapping::measureOD(int id) throw (std::runtime_error) {
	double vuelta = -1.0;

	switch (operation) {
	case mapping::sketch:
		sketching_measureOD(id);
		break;
	case mapping::exec_general:
		vuelta = exec_measureOD(id);
		break;
	case mapping::exec_ep:
		vuelta = exec_measureOD(id);
		break;
	default:
		break;
	}
	return vuelta;
}

void Mapping::loadContainer(int containerID, double volume) {
	switch (operation) {
	case mapping::sketch:
		sketching_loadContainer(containerID, volume);
		break;
	case mapping::exec_general:
		exec_loadContainer(containerID, volume);
		break;
	case mapping::exec_ep:
		exec_loadContainer(containerID, volume);
		break;
	default:
		break;
	}
}

double Mapping::timeStept() {
	double vuelta = -1.0;
	switch (operation) {
	case mapping::sketch:
		vuelta = sketching_timeStep();
		break;
	case mapping::exec_general:
		vuelta = exec_timeStep();
		break;
	case mapping::exec_ep:
		vuelta = exec_timeStep();
		break;
	default:
		break;
	}
	return vuelta;
}

//SKETCH
void Mapping::sketching_setContinuosFlow(int idSource, int idTarget,
	double rate) {
	LOG(DEBUG) << "sketching setContinousFlow(" << patch::to_string(idSource) << ", "
		<< patch::to_string(idTarget) << ", " + patch::to_string(rate)
		<< ")";
	//Update source node
	if (sketch->existsContainer(idSource)) {
		MachineGraph::ContainerNodePtr sourceNode = sketch->getContainer(idSource);
		if (sourceNode->getType().get()->isCompatibleMovement(MovementType::continuous)) {
			transformSourceContainer(idSource, idTarget, sourceNode, MovementType::continuous);
		}
		else {
			LOG(FATAL) << "container: " << patch::to_string(idTarget) << " cannot be continuous because already is" << sourceNode->getType().get()->getMovementTypeString();
		}
	}
	else {
		sketch->addContainer(idSource, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(MovementType::continuous, ContainerType::inlet)), 0.0);
	}

	// Update target node
	if (sketch->existsContainer(idTarget)) {
		MachineGraph::ContainerNodePtr targetNode = sketch->getContainer(idTarget);
		transformTargetContainer(idSource, idTarget, targetNode);
	}
	else {
		sketch->addContainer(idTarget, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(MovementType::irrelevant, ContainerType::sink)), 0.0);
	}
	/**** Connect source and target nodes ****/
	sketch->connectContainer(idSource, idTarget);
}

void Mapping::sketching_transfer(int idSource, int idTarget, double volume) {
	LOG(DEBUG) << "sketching transfer(" << patch::to_string(idSource) << ", "
		<< patch::to_string(idTarget) << ", " << patch::to_string(volume)
		<< ")";
	//Update source node
	if (sketch->existsContainer(idSource)) {
		MachineGraph::ContainerNodePtr sourceNode = sketch->getContainer(idSource);
		if (sourceNode->getType().get()->isCompatibleMovement(MovementType::discrete)) {
			transformSourceContainer(idSource, idTarget, sourceNode, MovementType::discrete);
		}
		else {
			LOG(FATAL) << "container: " << patch::to_string(idTarget) << " cannot be discrete because already is" << sourceNode->getType().get()->getMovementTypeString();
		}
	}
	else {
		sketch->addContainer(idSource, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(MovementType::discrete, ContainerType::inlet)), 0.0);
	}

	// Update target node
	if (sketch->existsContainer(idTarget)) {
		MachineGraph::ContainerNodePtr targetNode = sketch->getContainer(idTarget);
		transformTargetContainer(idSource, idTarget, targetNode);
	}
	else {
		sketch->addContainer(idTarget, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(MovementType::irrelevant, ContainerType::sink)), 0.0);
	}
	/**** Connect source and target nodes ****/
	sketch->connectContainer(idSource, idTarget);
}

void Mapping::transformSourceContainer(int idSource, int idTarget,
	MachineGraph::ContainerNodePtr sourceNode, MovementType desiredType) {

	switch (sourceNode->getType().get()->getContainerType()) {
	case ContainerType::unknow:
		sketch->changeContainerType(idSource, ContainerType::inlet);
		sketch->changeMovementType(idSource, desiredType);
		break;
	case ContainerType::inlet:
		if (!sketch->areConected(idSource, idTarget)) {
			sketch->changeContainerType(idSource,
				ContainerType::divergent_switch);
		}
		break;
	case ContainerType::flow:
		if (!sketch->areConected(idSource, idTarget)) {
			sketch->changeContainerType(idSource,
				ContainerType::divergent_switch_sink);
		}
		break;
	case ContainerType::sink:
		sketch->changeContainerType(idSource, ContainerType::flow);
		sketch->changeMovementType(idSource, desiredType);
		break;
	case ContainerType::convergent_switch:
		sketch->changeContainerType(idSource,
			ContainerType::convergent_switch_inlet);
		sketch->changeMovementType(idSource, desiredType);
		break;
	case ContainerType::convergent_switch_inlet:
		if (!sketch->areConected(idSource, idTarget)) {
			sketch->changeContainerType(idSource,
				ContainerType::bidirectional_switch);
		}
		break;
	default:
		break;
	}
}

void Mapping::transformTargetContainer(int idSource, int idTarget,
	MachineGraph::ContainerNodePtr targetNode) {

	switch (targetNode->getType().get()->getContainerType()) {
	case ContainerType::unknow:
		sketch->changeContainerType(idTarget, ContainerType::sink);
		break;
	case ContainerType::inlet:
		sketch->changeContainerType(idTarget, ContainerType::flow);
		break;
	case ContainerType::sink:
		if (!sketch->areConected(idSource, idTarget)) {
			sketch->changeContainerType(idTarget,
				ContainerType::convergent_switch);
		}
		break;
	case ContainerType::flow:
		if (!sketch->areConected(idSource, idTarget)) {
			sketch->changeContainerType(idTarget,
				ContainerType::convergent_switch_inlet);
		}
		break;
	case ContainerType::divergent_switch:
		sketch->changeContainerType(idTarget,
			ContainerType::divergent_switch_sink);
		break;
	case ContainerType::divergent_switch_sink:
		if (!sketch->areConected(idSource, idTarget)) {
			sketch->changeContainerType(idTarget,
				ContainerType::bidirectional_switch);
		}
		break;
	default:
		break;
	}
}

void Mapping::sketching_mix(int source1, int source2, int target,
	double volume1, double volume2) {
	LOG(DEBUG) << "sketching mix(" << patch::to_string(source1) << ", " << patch::to_string(source2) << ", "
		<< patch::to_string(target) + ", " << patch::to_string(volume1) << ", " << patch::to_string(volume2)
		<< ")";

	sketching_transfer(source1, target, volume1);
	sketching_transfer(source2, target, volume2);
}

void Mapping::sketching_loadContainer(int containerID, double volume) {
	LOG(DEBUG) << "sketching loadContainer(" << patch::to_string(containerID) << ", "
		<< patch::to_string(volume)
		<< ")";
	if (sketch->existsContainer(containerID)) {
		MachineGraph::ContainerNodePtr node = sketch->getContainer(containerID);
		node->setVolume(node->getVolume() + volume);
	}
	else {
		sketch->addContainer(containerID, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(MovementType::irrelevant, ContainerType::unknow)), volume);
	}
}

void Mapping::sketching_applyLight(int id, double wavelength,
	double intensity) {
	LOG(DEBUG) << "sketching applyLight(" << patch::to_string(id) << ", "
		<< patch::to_string(wavelength) << ", " << patch::to_string(intensity)
		<< ")";

	if (sketch->existsContainer(id)) {
		MachineGraph::ContainerNodePtr node = sketch->getContainer(id);
		node->getType().get()->addAddon(AddOnsType::light);
	}
	else {
		vector<AddOnsType> vect;
		vect.push_back(AddOnsType::light);
		sketch->addContainer(id, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(MovementType::irrelevant, ContainerType::unknow, vect)), 0.0);
	}
}

void Mapping::sketching_applyTemperature(int id, double degres) {
	LOG(DEBUG) << "sketching applyTemperature(" << patch::to_string(id) << ", "
		<< patch::to_string(degres) << ")";

	if (sketch->existsContainer(id)) {
		MachineGraph::ContainerNodePtr node = sketch->getContainer(id);
		node->getType().get()->addAddon(AddOnsType::temp);
	}
	else {
		vector<AddOnsType> vect;
		vect.push_back(AddOnsType::temp);
		sketch->addContainer(id, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(MovementType::irrelevant, ContainerType::unknow, vect)), 0.0);
	}
}

void Mapping::sketching_measureOD(int id) {
	LOG(DEBUG) << "sketching measureOD(" << patch::to_string(id) << ")";
	if (sketch->existsContainer(id)) {
		MachineGraph::ContainerNodePtr node = sketch->getContainer(id);
		node->getType().get()->addAddon(AddOnsType::OD_sensor);
	}
	else {
		vector<AddOnsType> vect;
		vect.push_back(AddOnsType::OD_sensor);
		sketch->addContainer(id, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(MovementType::irrelevant, ContainerType::unknow, vect)), 0.0);
	}
}

void Mapping::sketching_stir(int id) {
	LOG(DEBUG) << "sketching stir(" << patch::to_string(id) << "," << patch::to_string(-1) << ")";
	if (sketch->existsContainer(id)) {
		MachineGraph::ContainerNodePtr node = sketch->getContainer(id);
		node->getType().get()->addAddon(AddOnsType::mixer);
	}
	else {
		vector<AddOnsType> vect;
		vect.push_back(AddOnsType::mixer);
		sketch->addContainer(id, std::shared_ptr<ContainerNodeType>(new ContainerNodeType(MovementType::irrelevant, ContainerType::unknow, vect)), 0.0);
	}
}

double Mapping::sketching_timeStep() {
	LOG(DEBUG) << "sketching timeStept";
	return 1;
}

//EXEC
void Mapping::exec_setContinuosFlow(int idSource, int idTarget, double rate) throw (std::runtime_error) {
    LOG(INFO) << "exec setContinousFlow(" << patch::to_string(idSource) << ", "
		<< patch::to_string(idTarget) << ", " + patch::to_string(rate)
		<< ")";

	Flow<Edge>::FlowEdgePtr edge = std::make_shared<Edge>(idSource, idTarget);
	Flow<Edge>* flow = engine->getMappedEdge(edge);

	for (auto it = flow->getPaths().begin(); it != flow->getPaths().end();
		++it) {
		ExecutableMachineGraph::ExecutableContainerEdgePtr actual = *it;
		ExecutableMachineGraph::ExecutableContainerNodePtr source = machine->getContainer(
			actual->getIdSource());
		ExecutableMachineGraph::ExecutableContainerNodePtr target = machine->getContainer(
			actual->getIdTarget());

		if (source->getType()->hasMovementType(MovementType::continuous)) {
            if (rate > 0) {
                source->setPositionExtract(actual->getIdSource(), actual->getIdTarget());
                target->setPositionInject(actual->getIdSource(), actual->getIdTarget());
            }
			source->extractLiquid(rate);
			target->receiveLiquid(rate);

			cfEngine->setFlowRate(actual->getIdSource(), actual->getIdTarget(), rate);
		}
		else {
			LOG(FATAL) << "trying to do extract liquid with a continuous flow from a not continuous container, container id: " << patch::to_string(source->getContainerId());
		}
	}
}

void Mapping::exec_transfer(int idSource, int idTarget, double volume) throw (std::runtime_error) {
    LOG(INFO) << "exec transfer(" << patch::to_string(idSource) << ", "
		<< patch::to_string(idTarget) << ", " << patch::to_string(volume)
		<< ")";

	Flow<Edge>::FlowEdgePtr edge = std::make_shared<Edge>(idSource, idTarget);
	Flow<Edge>* flow = engine->getMappedEdge(edge);

	for (auto it = flow->getPaths().begin(); it != flow->getPaths().end();
		++it) {
		ExecutableMachineGraph::ExecutableContainerEdgePtr actual = *it;
		ExecutableMachineGraph::ExecutableContainerNodePtr source = machine->getContainer(
			actual->getIdSource());
		ExecutableMachineGraph::ExecutableContainerNodePtr target = machine->getContainer(
			actual->getIdTarget());

		if (source->getType()->hasMovementType(MovementType::discrete)) {
			source->setPositionExtract(actual->getIdSource(), actual->getIdTarget());
			target->setPositionInject(actual->getIdSource(), actual->getIdTarget());
			source->extractLiquid(volume);
			target->receiveLiquid(volume);
		}
		else {
			LOG(FATAL) << "trying to do extract liquid with a transfer flow from a not discrete container, container id: " << patch::to_string(source->getContainerId());
		}
	}
}

void Mapping::exec_mix(int source1, int source2, int target, double volume1, double volume2) throw (std::runtime_error) {
    LOG(INFO) << "exec mix(" << patch::to_string(source1) << ", " << patch::to_string(source2) << ", "
		<< patch::to_string(target) + ", " << patch::to_string(volume1) << ", " << patch::to_string(volume2)
		<< ")";

	exec_transfer(source1, target, volume1);
	exec_transfer(source2, target, volume2);
}

void Mapping::exec_applyLight(int id, double wavelength, double intensity) throw (std::runtime_error) {
    LOG(INFO) << "exec applyLight(" << patch::to_string(id) << ", "
		<< patch::to_string(wavelength) << ", " << patch::to_string(intensity)
		<< ")";

	ExecutableMachineGraph::ExecutableContainerNodePtr mappedContainer = machine->getContainer(engine->getMappedContainerId(id));
	std::shared_ptr<Light> light = mappedContainer->getLight();

	if (light.get() != NULL) {
		light.get()->applyLight(wavelength, intensity);
	}
	else {
		LOG(FATAL) << "trying to apply light to a container without that add on, container id: " << patch::to_string(mappedContainer->getContainerId());
	}
}

void Mapping::exec_applyTemperature(int id, double degres) throw (std::runtime_error) {
    LOG(INFO) << "exec applyTemperature(" << patch::to_string(id) << ", "
		<< patch::to_string(degres) << ")";

	ExecutableMachineGraph::ExecutableContainerNodePtr mappedContainer = machine->getContainer(
		engine->getMappedContainerId(id));
	std::shared_ptr<Temperature> temp = mappedContainer->getTemperature();

	if (temp.get() != NULL) {
		temp.get()->apply(degres);
	}
	else {
		LOG(FATAL) << "trying to apply temperature to a container without that add on, container id: " << patch::to_string(mappedContainer->getContainerId());
	}
}

double Mapping::exec_getVolume(int id) {
	return machine->getContainer(engine->getMappedContainerId(id))->getVolume();
}

double Mapping::exec_measureOD(int id) throw (std::runtime_error) {
	double measureValued = -1;
	ExecutableMachineGraph::ExecutableContainerNodePtr mappedContainer = machine->getContainer(
		engine->getMappedContainerId(id));
	std::shared_ptr<ODSensor> od = mappedContainer->getOd();

	if (od.get() != NULL) {
		measureValued = od.get()->readOd();
        LOG(INFO) << "exec measureOD(" << patch::to_string(id) << ") = " << patch::to_string(measureValued);
	}
	else {
		LOG(FATAL) << "trying to measure OD from a container without that add on, container id: " << patch::to_string(mappedContainer->getContainerId());
	}
	return measureValued;
}

void Mapping::exec_loadContainer(int containerID, double volume) {
    LOG(INFO) << "exec loadContainer(" << patch::to_string(containerID) << ", "
		<< patch::to_string(volume)
		<< ")";
	ExecutableMachineGraph::ExecutableContainerNodePtr mappedContainer = machine->getContainer(
		engine->getMappedContainerId(containerID));
	mappedContainer->setVolume(volume);
}

void Mapping::exec_stir(int id, double intensity) throw (std::runtime_error) {
    LOG(INFO) << "exec stir(" << patch::to_string(id) << "," << patch::to_string(intensity) << ")";
	ExecutableMachineGraph::ExecutableContainerNodePtr mappedContainer = machine->getContainer(
		engine->getMappedContainerId(id));
	std::shared_ptr<Mixer> mix = mappedContainer->getMix();

	if (mix.get() != NULL) {
		mix.get()->mix(intensity);
	}
	else {
		LOG(FATAL) << "trying to stir a container without that add on, container id: " << patch::to_string(mappedContainer->getContainerId());
	}
}

double Mapping::exec_timeStep() {
    LOG(INFO) << "executing timeStept:";
    long msPassed;
	if (this->testing) {
		msPassed = SLEEP_MS;
	}
	else {

		long tempTimestamp = Utils::getCurrentTimeMilis();
		msPassed = tempTimestamp - lastTimestamp;
		lastTimestamp = tempTimestamp;
		LOG(DEBUG) << "executing timeStept: passed " << msPassed << " ms";
	}

	cfEngine->updateVolumens(msPassed);

	return (msPassed);
}

//MISCELANEOUS
void Mapping::printSketch(const std::string& path) {
	sketch->printMachine(path);
}

void Mapping::startCommunications() {
	for (auto it = communicationsInterfaces->begin();
		it != communicationsInterfaces->end(); ++it) {
		CommandSender* com =
			CommunicationsInterface::GetInstance()->getCommandSender(*it);
		com->connect();
	}
}

void Mapping::stopCommunications() {
	for (auto it = communicationsInterfaces->begin();
		it != communicationsInterfaces->end(); ++it) {
		CommandSender* com =
			CommunicationsInterface::GetInstance()->getCommandSender(*it);
		com->disconnect();
	}
}

std::string Mapping::printMappingTable() {
    std::stringstream stream;
    const MachineGraph::ContainerEdgeVectorPtr edges = sketch->getGraph()->getEdgeList();
    const MachineGraph::ContainerNodeVectorPtr nodes = sketch->getGraph()->getAllNodes();

    for (auto it = nodes->begin(); it != nodes->end(); ++it) {
        MachineGraph::ContainerNodePtr act = *it;
        stream << " sketch : " << patch::to_string(act->getContainerId()) << ", machine: " << patch::to_string(engine->getMappedContainerId(act->getContainerId()));
    }

    for (auto it = edges->begin(); it != edges->end(); ++it) {
        MachineGraph::ContainerEdgePtr act = *it;
        stream << " sketch : " << act->toText() << ", machine: " << engine->getMappedEdge(act)->toText();
    }
    return stream.str();
}

void Mapping::cleanUsedResources() {
    engine->cleanUsedResources();
}
