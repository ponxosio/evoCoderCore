#-------------------------------------------------
#
# Project created by QtCreator 2016-09-30T10:22:11
#
#-------------------------------------------------

QT       -= gui

TARGET = EvoCoderCore
TEMPLATE = lib
#CONFIG+= staticlib

DEFINES += EVOCODERCORE_LIBRARY

INCLUDEPATH += X:/boost_1_61_0
INCLUDEPATH += X:/EvoCoderCore/lib
INCLUDEPATH += C:/Python27/include

LIBS += -L$$quote(X:\boost_1_61_0\stage\lib) -lboost_python-vc140-mt-1_61
LIBS += -L$$quote(C:\Python27\libs)

QMAKE_CXXFLAGS += -bigobj

SOURCES += \
    fluidControl/executable/containers/actuators/communications/CommunicationsInterface.cpp \
    fluidControl/executable/containers/actuators/communications/FileSender.cpp \
    fluidControl/executable/containers/actuators/communications/SerialSender.cpp \
    fluidControl/executable/containers/BidirectionalSwitch.cpp \
    fluidControl/executable/containers/ConvergentSwitch.cpp \
    fluidControl/executable/containers/ConvergentSwitchInlet.cpp \
    fluidControl/executable/containers/DivergentSwitch.cpp \
    fluidControl/executable/containers/DivergentSwitchSink.cpp \
    fluidControl/executable/containers/FlowContainer.cpp \
    fluidControl/executable/containers/InletContainer.cpp \
    fluidControl/executable/containers/SinkContainer.cpp \
    fluidControl/executable/ExecutableMachineGraph.cpp \
    fluidControl/machineGraph/ContainerNode.cpp \
    fluidControl/machineGraph/ContainerNodeType.cpp \
    fluidControl/machineGraph/MachineGraph.cpp \
    fluidControl/mapping/pathcalculator/IdSearcher.cpp \
    fluidControl/mapping/pathcalculator/PathManager.cpp \
    fluidControl/mapping/pathcalculator/PathSearcher.cpp \
    fluidControl/mapping/pathcalculator/PathSearcherIterator.cpp \
    fluidControl/mapping/pathcalculator/SearcherIterator.cpp \
    fluidControl/mapping/pathcalculator/SimplePathSearcher.cpp \
    fluidControl/mapping/pathcalculator/TypeSearcher.cpp \
    fluidControl/mapping/ContinuousFlowEngine.cpp \
    fluidControl/mapping/Mapping.cpp \
    fluidControl/mapping/MappingEngine.cpp \
    fluidControl/ExecutionEngine.cpp \
    operables/comparison/BooleanComparison.cpp \
    operables/comparison/SimpleComparison.cpp \
    operables/mathematics/ArithmeticOperation.cpp \
    operables/mathematics/ConstantNumber.cpp \
    operables/mathematics/UnaryOperation.cpp \
    operables/mathematics/VariableEntry.cpp \
    operables/VariableTable.cpp \
    plugin/actuators/ControlPlugin.cpp \
    plugin/actuators/ExtractorPlugin.cpp \
    plugin/actuators/InjectorPlugin.cpp \
    plugin/actuators/LightPlugin.cpp \
    plugin/actuators/MixerPlugin.cpp \
    plugin/actuators/ODSensorPlugin.cpp \
    plugin/actuators/selfconfiguringplugin.cpp \
    plugin/actuators/TemperaturePlugin.cpp \
    plugin/PluginFileLoader.cpp \
    plugin/PythonEnvironment.cpp \
    protocolGraph/operations/container/ApplyLight.cpp \
    protocolGraph/operations/container/ApplyTemperature.cpp \
    protocolGraph/operations/container/GetVolume.cpp \
    protocolGraph/operations/container/LoadContainerOperation.cpp \
    protocolGraph/operations/container/MeasureOD.cpp \
    protocolGraph/operations/container/Mix.cpp \
    protocolGraph/operations/container/SetContinousFlow.cpp \
    protocolGraph/operations/container/Stir.cpp \
    protocolGraph/operations/container/TimeStep.cpp \
    protocolGraph/operations/container/Transfer.cpp \
    protocolGraph/operations/AssignationOperation.cpp \
    protocolGraph/operations/DivergeNode.cpp \
    protocolGraph/operations/LoopNode.cpp \
    protocolGraph/ConditionEdge.cpp \
    protocolGraph/ProtocolGraph.cpp \
    util/AutoEnumerate.cpp \
    util/Utils.cpp \
    BioBlocksJSONReader.cpp \
    ExecutionMachineServer.cpp \
    ExecutionServer.cpp

HEADERS +=\
        evocodercore_global.h \
    fluidControl/executable/containers/actuators/communications/CommandSender.h \
    fluidControl/executable/containers/actuators/communications/CommunicationsInterface.h \
    fluidControl/executable/containers/actuators/communications/FileSender.h \
    fluidControl/executable/containers/actuators/communications/SerialSender.h \
    fluidControl/executable/containers/actuators/extras/Light.h \
    fluidControl/executable/containers/actuators/extras/Mixer.h \
    fluidControl/executable/containers/actuators/extras/ODSensor.h \
    fluidControl/executable/containers/actuators/extras/Temperature.h \
    fluidControl/executable/containers/actuators/liquids/Control.h \
    fluidControl/executable/containers/actuators/liquids/Extractor.h \
    fluidControl/executable/containers/actuators/liquids/Injector.h \
    fluidControl/executable/containers/actuators/Instructable.h \
    fluidControl/executable/containers/BidirectionalSwitch.h \
    fluidControl/executable/containers/ConvergentSwitch.h \
    fluidControl/executable/containers/ConvergentSwitchInlet.h \
    fluidControl/executable/containers/DivergentSwitch.h \
    fluidControl/executable/containers/DivergentSwitchSink.h \
    fluidControl/executable/containers/ExecutableContainerNode.h \
    fluidControl/executable/containers/FlowContainer.h \
    fluidControl/executable/containers/InletContainer.h \
    fluidControl/executable/containers/SinkContainer.h \
    fluidControl/executable/ExecutableMachineGraph.h \
    fluidControl/machineGraph/ContainerNode.h \
    fluidControl/machineGraph/ContainerNodeType.h \
    fluidControl/machineGraph/MachineGraph.h \
    fluidControl/mapping/pathcalculator/IdSearcher.h \
    fluidControl/mapping/pathcalculator/PathManager.h \
    fluidControl/mapping/pathcalculator/PathSearcher.h \
    fluidControl/mapping/pathcalculator/PathSearcherIterator.h \
    fluidControl/mapping/pathcalculator/SearcherInterface.h \
    fluidControl/mapping/pathcalculator/SearcherIterator.h \
    fluidControl/mapping/pathcalculator/SimplePathSearcher.h \
    fluidControl/mapping/pathcalculator/TypeSearcher.h \
    fluidControl/mapping/ContinuousFlowEngine.h \
    fluidControl/mapping/Mapping.h \
    fluidControl/mapping/MappingEngine.h \
    fluidControl/ExecutionEngine.h \
    graph/Edge.h \
    graph/Flow.h \
    graph/FlowPtrComparator.h \
    graph/Graph.h \
    graph/Node.h \
    operables/comparison/BooleanComparison.h \
    operables/comparison/ComparisonOperable.h \
    operables/comparison/SimpleComparison.h \
    operables/comparison/Tautology.h \
    operables/mathematics/ArithmeticOperation.h \
    operables/mathematics/ConstantNumber.h \
    operables/mathematics/MathematicOperable.h \
    operables/mathematics/UnaryOperation.h \
    operables/mathematics/VariableEntry.h \
    operables/ComparisonOperable.h \
    operables/VariableTable.h \
    plugin/actuators/ControlPlugin.h \
    plugin/actuators/ExtractorPlugin.h \
    plugin/actuators/InjectorPlugin.h \
    plugin/actuators/LightPlugin.h \
    plugin/actuators/MixerPlugin.h \
    plugin/actuators/ODSensorPlugin.h \
    plugin/actuators/selfconfiguringplugin.h \
    plugin/actuators/TemperaturePlugin.h \
    plugin/PluginFileLoader.h \
    plugin/PythonEnvironment.h \
    protocolGraph/operations/container/ApplyLight.h \
    protocolGraph/operations/container/ApplyTemperature.h \
    protocolGraph/operations/container/ContainerOperation.h \
    protocolGraph/operations/container/GetVolume.h \
    protocolGraph/operations/container/LoadContainerOperation.h \
    protocolGraph/operations/container/MeasureOD.h \
    protocolGraph/operations/container/Mix.h \
    protocolGraph/operations/container/SetContinousFlow.h \
    protocolGraph/operations/container/Stir.h \
    protocolGraph/operations/container/TimeStep.h \
    protocolGraph/operations/container/Transfer.h \
    protocolGraph/operations/AssignationOperation.h \
    protocolGraph/operations/DivergeNode.h \
    protocolGraph/operations/LoopNode.h \
    protocolGraph/ConditionEdge.h \
    protocolGraph/OperationNode.h \
    protocolGraph/ProtocolGraph.h \
    util/AutoEnumerate.h \
    util/ContainersUtils.h \
    util/Patch.h \
    util/Utils.h \
    BioBlocksJSONReader.h \
    ExecutionMachineServer.h \
    ExecutionServer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    extraresources.qrc
