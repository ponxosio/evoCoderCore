#ifndef EVOCODERCORE_GLOBAL_H
#define EVOCODERCORE_GLOBAL_H

#include <easylogging++.h>

#include <QtCore/qglobal.h>

#if defined(EVOCODERCORE_LIBRARY)
#	define BIOBLOCKSJSONREADER_EXPORT Q_DECL_EXPORT
#	define EXECUTIONMACHINESERVER_EXPORT Q_DECL_EXPORT
#	define EXECUTIONSERVER_EXPORT Q_DECL_EXPORT
#	define COMMANDSENDER_EXPORT Q_DECL_EXPORT
#	define COMMUNICATIONSINTERFACE_EXPORT Q_DECL_EXPORT
#	define FILESENDER_EXPORT Q_DECL_EXPORT
#	define SERIALSENDER_EXPORT Q_DECL_EXPORT
#	define LIGHT_EXPORT Q_DECL_EXPORT
#	define MIXER_EXPORT Q_DECL_EXPORT
#	define ODSENSOR_EXPORT Q_DECL_EXPORT
#	define TEMPERATURE_EXPORT Q_DECL_EXPORT
#	define INSTRUCTABLE_EXPORT Q_DECL_EXPORT
#	define CONTROL_EXPORT Q_DECL_EXPORT
#	define EXTRACTOR_EXPORT Q_DECL_EXPORT
#	define INJECTOR_EXPORT Q_DECL_EXPORT
#	define BIDIRECTIONALSWITCH_EXPORT Q_DECL_EXPORT
#	define CONVERGENTSWITCH_EXPORT Q_DECL_EXPORT
#	define CONVERGENTSWITCHINLET_EXPORT Q_DECL_EXPORT
#	define DIVERGENTSWITCH_EXPORT Q_DECL_EXPORT
#	define DIVERGENTSWITCHSINK_EXPORT Q_DECL_EXPORT
#	define EXECUTABLECONTAINERNODE_EXPORT Q_DECL_EXPORT
#	define FLOWCONTAINER_EXPORT Q_DECL_EXPORT
#	define INLETCONTAINER_EXPORT Q_DECL_EXPORT
#	define SINKCONTAINER_EXPORT Q_DECL_EXPORT
#	define EXECUTABLEMACHINEGRAPH_EXPORT Q_DECL_EXPORT
#	define EXECUTIONENGINE_EXPORT Q_DECL_EXPORT
#	define CONTAINERNODE_EXPORT Q_DECL_EXPORT
#	define CONTAINERNODETYPE_EXPORT Q_DECL_EXPORT
#	define MACHINEGRAPH_EXPORT Q_DECL_EXPORT
#	define CONTINUOUSFLOWENGINE_EXPORT Q_DECL_EXPORT
#	define MAPPING_EXPORT Q_DECL_EXPORT
#	define MAPPINGENGINE_EXPORT Q_DECL_EXPORT
#	define IDSEARCHER_EXPORT Q_DECL_EXPORT
#	define PATHMANAGER_EXPORT Q_DECL_EXPORT
#	define PATHSEARCHER_EXPORT Q_DECL_EXPORT
#	define PATHSEARCHERITERATOR_EXPORT Q_DECL_EXPORT
#	define SEARCHERINTERFACE_EXPORT Q_DECL_EXPORT
#	define SEARCHERITERATOR_EXPORT Q_DECL_EXPORT
#	define SIMPLEPATHSEARCHER_EXPORT Q_DECL_EXPORT
#	define TYPESEARCHER_EXPORT Q_DECL_EXPORT
#	define EDGE_EXPORT Q_DECL_EXPORT
#	define NODE_EXPORT Q_DECL_EXPORT
#	define BOOLEANCOMPARISON_EXPORT Q_DECL_EXPORT
#	define COMPARISONOPERABLE_EXPORT Q_DECL_EXPORT
#	define SIMPLECOMPARISON_EXPORT Q_DECL_EXPORT
#	define TAUTOLOGY_EXPORT Q_DECL_EXPORT
#	define COMPARISONOPERABLE_EXPORT Q_DECL_EXPORT
#	define ARITHMETICOPERATION_EXPORT Q_DECL_EXPORT
#	define CONSTANTNUMBER_EXPORT Q_DECL_EXPORT
#	define MATHEMATICOPERABLE_EXPORT Q_DECL_EXPORT
#	define UNARYOPERATION_EXPORT Q_DECL_EXPORT
#	define VARIABLEENTRY_EXPORT Q_DECL_EXPORT
#	define VARIABLETABLE_EXPORT Q_DECL_EXPORT
#	define CONTROLPLUGIN_EXPORT Q_DECL_EXPORT
#	define EXTRACTORPLUGIN_EXPORT Q_DECL_EXPORT
#	define INJECTORPLUGIN_EXPORT Q_DECL_EXPORT
#	define LIGHTPLUGIN_EXPORT Q_DECL_EXPORT
#	define MIXERPLUGIN_EXPORT Q_DECL_EXPORT
#	define ODSENSORPLUGIN_EXPORT Q_DECL_EXPORT
#	define SELFCONFIGURINGPLUGIN_EXPORT Q_DECL_EXPORT
#	define TEMPERATUREPLUGIN_EXPORT Q_DECL_EXPORT
#	define PLUGINFILELOADER_EXPORT Q_DECL_EXPORT
#	define PYTHONENVIRONMENT_EXPORT Q_DECL_EXPORT
#	define CONDITIONEDGE_EXPORT Q_DECL_EXPORT
#	define OPERATIONNODE_EXPORT Q_DECL_EXPORT
#	define ASSIGNATIONOPERATION_EXPORT Q_DECL_EXPORT
#	define APPLYLIGHT_EXPORT Q_DECL_EXPORT
#	define APPLYTEMPERATURE_EXPORT Q_DECL_EXPORT
#	define CONTAINEROPERATION_EXPORT Q_DECL_EXPORT
#	define GETVOLUME_EXPORT Q_DECL_EXPORT
#	define LOADCONTAINEROPERATION_EXPORT Q_DECL_EXPORT
#	define MEASUREOD_EXPORT Q_DECL_EXPORT
#	define MIX_EXPORT Q_DECL_EXPORT
#	define SETCONTINOUSFLOW_EXPORT Q_DECL_EXPORT
#	define STIR_EXPORT Q_DECL_EXPORT
#	define TIMESTEP_EXPORT Q_DECL_EXPORT
#	define TRANSFER_EXPORT Q_DECL_EXPORT
#	define DIVERGENODE_EXPORT Q_DECL_EXPORT
#	define LOOPNODE_EXPORT Q_DECL_EXPORT
#	define PROTOCOLGRAPH_EXPORT Q_DECL_EXPORT
#	define AUTOENUMERATE_EXPORT Q_DECL_EXPORT
#	define CONTAINERSUTILS_EXPORT Q_DECL_EXPORT
#	define UTILS_EXPORT Q_DECL_EXPORT
#   define FLOW_EXPORT Q_DECL_EXPORT
#   define FLOWPTRCOMPARATOR_EXPORT Q_DECL_EXPORT
#   define GRAPH_EXPORT Q_DECL_EXPORT
#   define PATCH_EXPORT Q_DECL_EXPORT
#   define LOGUTILS_EXPORT Q_DECL_EXPORT
#   define COMPOUNDCONTROLPLUGIN_EXPORT Q_DECL_EXPORT
#   define VALVECONTROLLEDTWINPUMP_EXPORT Q_DECL_EXPORT
#   define SETTIMESTEP_EXPORT Q_DECL_EXPORT
#   define CONDITIONALFLOWEDGE_EXPORT Q_DECL_EXPORT
# else
#	define BIOBLOCKSJSONREADER_EXPORT Q_DECL_IMPORT
#	define EXECUTIONMACHINESERVER_EXPORT Q_DECL_IMPORT
#	define EXECUTIONSERVER_EXPORT Q_DECL_IMPORT
#	define COMMANDSENDER_EXPORT Q_DECL_IMPORT
#	define COMMUNICATIONSINTERFACE_EXPORT Q_DECL_IMPORT
#	define FILESENDER_EXPORT Q_DECL_IMPORT
#	define SERIALSENDER_EXPORT Q_DECL_IMPORT
#	define LIGHT_EXPORT Q_DECL_IMPORT
#	define MIXER_EXPORT Q_DECL_IMPORT
#	define ODSENSOR_EXPORT Q_DECL_IMPORT
#	define TEMPERATURE_EXPORT Q_DECL_IMPORT
#	define INSTRUCTABLE_EXPORT Q_DECL_IMPORT
#	define CONTROL_EXPORT Q_DECL_IMPORT
#	define EXTRACTOR_EXPORT Q_DECL_IMPORT
#	define INJECTOR_EXPORT Q_DECL_IMPORT
#	define BIDIRECTIONALSWITCH_EXPORT Q_DECL_IMPORT
#	define CONVERGENTSWITCH_EXPORT Q_DECL_IMPORT
#	define CONVERGENTSWITCHINLET_EXPORT Q_DECL_IMPORT
#	define DIVERGENTSWITCH_EXPORT Q_DECL_IMPORT
#	define DIVERGENTSWITCHSINK_EXPORT Q_DECL_IMPORT
#	define EXECUTABLECONTAINERNODE_EXPORT Q_DECL_IMPORT
#	define FLOWCONTAINER_EXPORT Q_DECL_IMPORT
#	define INLETCONTAINER_EXPORT Q_DECL_IMPORT
#	define SINKCONTAINER_EXPORT Q_DECL_IMPORT
#	define EXECUTABLEMACHINEGRAPH_EXPORT Q_DECL_IMPORT
#	define EXECUTIONENGINE_EXPORT Q_DECL_IMPORT
#	define CONTAINERNODE_EXPORT Q_DECL_IMPORT
#	define CONTAINERNODETYPE_EXPORT Q_DECL_IMPORT
#	define MACHINEGRAPH_EXPORT Q_DECL_IMPORT
#	define CONTINUOUSFLOWENGINE_EXPORT Q_DECL_IMPORT
#	define MAPPING_EXPORT Q_DECL_IMPORT
#	define MAPPINGENGINE_EXPORT Q_DECL_IMPORT
#	define IDSEARCHER_EXPORT Q_DECL_IMPORT
#	define PATHMANAGER_EXPORT Q_DECL_IMPORT
#	define PATHSEARCHER_EXPORT Q_DECL_IMPORT
#	define PATHSEARCHERITERATOR_EXPORT Q_DECL_IMPORT
#	define SEARCHERINTERFACE_EXPORT Q_DECL_IMPORT
#	define SEARCHERITERATOR_EXPORT Q_DECL_IMPORT
#	define SIMPLEPATHSEARCHER_EXPORT Q_DECL_IMPORT
#	define TYPESEARCHER_EXPORT Q_DECL_IMPORT
#	define EDGE_EXPORT Q_DECL_IMPORT
#	define NODE_EXPORT Q_DECL_IMPORT
#	define BOOLEANCOMPARISON_EXPORT Q_DECL_IMPORT
#	define COMPARISONOPERABLE_EXPORT Q_DECL_IMPORT
#	define SIMPLECOMPARISON_EXPORT Q_DECL_IMPORT
#	define TAUTOLOGY_EXPORT Q_DECL_IMPORT
#	define COMPARISONOPERABLE_EXPORT Q_DECL_IMPORT
#	define ARITHMETICOPERATION_EXPORT Q_DECL_IMPORT
#	define CONSTANTNUMBER_EXPORT Q_DECL_IMPORT
#	define MATHEMATICOPERABLE_EXPORT Q_DECL_IMPORT
#	define UNARYOPERATION_EXPORT Q_DECL_IMPORT
#	define VARIABLEENTRY_EXPORT Q_DECL_IMPORT
#	define VARIABLETABLE_EXPORT Q_DECL_IMPORT
#	define CONTROLPLUGIN_EXPORT Q_DECL_IMPORT
#	define EXTRACTORPLUGIN_EXPORT Q_DECL_IMPORT
#	define INJECTORPLUGIN_EXPORT Q_DECL_IMPORT
#	define LIGHTPLUGIN_EXPORT Q_DECL_IMPORT
#	define MIXERPLUGIN_EXPORT Q_DECL_IMPORT
#	define ODSENSORPLUGIN_EXPORT Q_DECL_IMPORT
#	define SELFCONFIGURINGPLUGIN_EXPORT Q_DECL_IMPORT
#	define TEMPERATUREPLUGIN_EXPORT Q_DECL_IMPORT
#	define PLUGINFILELOADER_EXPORT Q_DECL_IMPORT
#	define PYTHONENVIRONMENT_EXPORT Q_DECL_IMPORT
#	define CONDITIONEDGE_EXPORT Q_DECL_IMPORT
#	define OPERATIONNODE_EXPORT Q_DECL_IMPORT
#	define ASSIGNATIONOPERATION_EXPORT Q_DECL_IMPORT
#	define APPLYLIGHT_EXPORT Q_DECL_IMPORT
#	define APPLYTEMPERATURE_EXPORT Q_DECL_IMPORT
#	define CONTAINEROPERATION_EXPORT Q_DECL_IMPORT
#	define GETVOLUME_EXPORT Q_DECL_IMPORT
#	define LOADCONTAINEROPERATION_EXPORT Q_DECL_IMPORT
#	define MEASUREOD_EXPORT Q_DECL_IMPORT
#	define MIX_EXPORT Q_DECL_IMPORT
#	define SETCONTINOUSFLOW_EXPORT Q_DECL_IMPORT
#	define STIR_EXPORT Q_DECL_IMPORT
#	define TIMESTEP_EXPORT Q_DECL_IMPORT
#	define TRANSFER_EXPORT Q_DECL_IMPORT
#	define DIVERGENODE_EXPORT Q_DECL_IMPORT
#	define LOOPNODE_EXPORT Q_DECL_IMPORT
#	define PROTOCOLGRAPH_EXPORT Q_DECL_IMPORT
#	define AUTOENUMERATE_EXPORT Q_DECL_IMPORT
#	define CONTAINERSUTILS_EXPORT Q_DECL_IMPORT
#	define UTILS_EXPORT Q_DECL_IMPORT
#   define FLOW_EXPORT Q_DECL_IMPORT
#   define FLOWPTRCOMPARATOR_EXPORT Q_DECL_IMPORT
#   define GRAPH_EXPORT Q_DECL_IMPORT
#   define PATCH_EXPORT Q_DECL_IMPORT
#   define LOGUTILS_EXPORT Q_DECL_IMPORT
#   define COMPOUNDCONTROLPLUGIN_EXPORT Q_DECL_IMPORT
#   define VALVECONTROLLEDTWINPUMP_EXPORT Q_DECL_IMPORT
#   define SETTIMESTEP_EXPORT Q_DECL_IMPORT
#   define CONDITIONALFLOWEDGE_EXPORT Q_DECL_IMPORT
#endif

#endif // EVOCODERCORE_GLOBAL_H
