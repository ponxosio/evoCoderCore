# include "ContainerOperation.h"
# include "ExecutionServer.h"

ContainerOperation::ContainerOperation() :
    OperationNode()
{

}
ContainerOperation::ContainerOperation(const ContainerOperation & obj) :
    OperationNode(obj)
{

}
ContainerOperation::ContainerOperation(int idConatiner) :
    OperationNode(idConatiner)
{

}

ContainerOperation::~ContainerOperation() {

}

void ContainerOperation::updateReference(const std::string & reference) {
    this->reference = reference;
}

std::shared_ptr<Mapping> ContainerOperation::getMapping() throw (std::invalid_argument) {
    try {
        return ExecutionServer::GetInstance()->getEvoCoder(reference)->getMapping();
    }
    catch (std::invalid_argument & e) {
        throw(std::invalid_argument("ContainerOperation::getMapping(), " + std::string(e.what())));
    }
}
