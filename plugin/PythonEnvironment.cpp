#include "PythonEnvironment.h"

PythonEnvironment *PythonEnvironment::m_pInstance = NULL;

BOOST_PYTHON_MODULE(communicationsMod)
{
	boost::python::class_<CommandSenderWrap, boost::noncopyable>("CommandSender", boost::python::no_init)
		.def("sendString", boost::python::pure_virtual(&CommandSender::sendString))
		.def("receiveString", boost::python::pure_virtual(&CommandSender::receiveString))
		.def("readUntil", boost::python::pure_virtual(&CommandSender::readUntil))
		.def("synch", boost::python::pure_virtual(&CommandSender::synch))
		.def("connect", boost::python::pure_virtual(&CommandSender::connect))
		.def("disconnect", boost::python::pure_virtual(&CommandSender::disconnect))
		;

	boost::python::class_<SerialSender, boost::python::bases<CommandSender>, boost::noncopyable>("SerialSender", boost::python::no_init)
		.def("sendString", &SerialSender::sendString)
		.def("receiveString", &SerialSender::receiveString)
		.def("readUntil", &SerialSender::readUntil)
		.def("synch", &SerialSender::synch)
		.def("connect", &SerialSender::connect)
		.def("disconnect", &SerialSender::disconnect)
		;

	boost::python::class_<FileSender, boost::python::bases<CommandSender>, boost::noncopyable>("FileSender", boost::python::no_init)
		.def("sendString", &FileSender::sendString)
		.def("receiveString", &FileSender::receiveString)
		.def("readUntil", &FileSender::readUntil)
		.def("synch", &FileSender::synch)
		.def("connect", &FileSender::connect)
		.def("disconnect", &FileSender::disconnect)
		;
}

namespace boost { template <> CommandSender const volatile * get_pointer(class CommandSender const volatile *c) { return c; } }

using namespace boost::python;

PythonEnvironment::PythonEnvironment()
{
}


PythonEnvironment::~PythonEnvironment()
{
}

void PythonEnvironment::addImportPath(const std::string & path)
{
	//Add plugins folder to the python path
    std::string wordPath = "path";
    boost::scoped_array<char> writable(new char[wordPath.size() + 1]);
    std::copy(wordPath.begin(), wordPath.end(), writable.get());
    writable[wordPath.size()] = '\0'; // don't forget the terminating 0

    PyObject* sysPath = PySys_GetObject(writable.get());
	PyList_Insert(sysPath, 0, PyString_FromString(path.c_str()));
}

std::string PythonEnvironment::makeInstance(const std::string & type, const std::unordered_map<std::string,std::string>& params) throw (std::invalid_argument)
{
	try {
		std::string path = PluginFileLoader::GetInstance()->getPath(type);
		std::string dir = getDirectoryPath(path);
		if (loadedFiles.find(dir) == loadedFiles.end()) {
			LOG(DEBUG) << "adding path: " << dir;
			addImportPath(dir);
			loadedFiles.insert(dir);
		}

		object importObj;
		std::string importName = getPythonClassNAme(path);
		auto it = loadedClasses.find(importName);
		if (it == loadedClasses.end()) {
			LOG(DEBUG) << "importing file: " << importName;
			importObj = import(str(importName));
			loadedClasses.insert(std::make_pair(importName, importObj));
		}
		else {
			importObj = it->second;
		}

        boost::python::dict d;
        for (auto it = params.begin(); it != params.end(); ++it) {
            d[it->first] = it->second;
        }

		LOG(DEBUG) << "making new var name...";
		std::string varName = "var" + patch::to_string(this->autoEnum.getNextValue());
		LOG(DEBUG) << "creating new var on the environment...";
        this->main_namespace[varName] = importObj.attr(str(type))(d);
		availableInstances.insert(varName);

		return varName;
	}
	catch (std::invalid_argument & e) {
		throw(e);
	}
	catch (error_already_set) {
		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);
		
		char * cs = PyString_AsString(pvalue);
		
		std::string msg = "unknow";
		if (cs) {
			msg = std::string(cs);
		}

		throw(std::invalid_argument("error at python environment " + msg));
	}
}

void PythonEnvironment::deleteInstance(const std::string & varName) {
	if (availableInstances.find(varName) != availableInstances.end()) {
		try {
			this->main_namespace[varName].del();
		}
		catch (error_already_set) {
			PyObject *ptype, *pvalue, *ptraceback;
			PyErr_Fetch(&ptype, &pvalue, &ptraceback);
			char * cs = PyString_AsString(pvalue);

			std::string msg = "unknow";
			if (cs) {
				msg = std::string(cs);
			}

			throw(std::invalid_argument("error at python environment " + msg));
		}
	}
	else {
		LOG(WARNING) << "tying to remove an unistantiate variable: " << varName;
	}
}

boost::python::api::object PythonEnvironment::getVarInstance(const std::string & varName) throw (std::invalid_argument)
{
	if (availableInstances.find(varName) != availableInstances.end()) {
		try {
			LOG(DEBUG) << "getting variable " << varName << " from the main_namespace...";
			return this->main_namespace[varName];
		}
		catch (error_already_set) {
			PyObject *ptype, *pvalue, *ptraceback;
			PyErr_Fetch(&ptype, &pvalue, &ptraceback);

			char * cs = PyString_AsString(pvalue);

			std::string msg = "unknow";
			if (cs) {
				msg = std::string(cs);
			}

			throw(std::invalid_argument("error at python environment " + msg));
		}
	}
	else {
		LOG(WARNING) << "tying to get an unistantiate variable: " << varName;
	}
}

void PythonEnvironment::initEnvironment()
{
	LOG(DEBUG) << "initializing environment...";
	Py_Initialize();

	initcommunicationsMod();

	object main_module = import("__main__");
	this->main_namespace = main_module.attr("__dict__");

	//create types used in the execution
	class_<std::vector<std::string>>("ParamsList")
		.def(vector_indexing_suite<std::vector<std::string>>());

	addImportPath(Utils::getCurrentDir() + "\\" + BASE_PLUGIN_FOLDER);
}

void PythonEnvironment::finishEnvironment()
{
	LOG(DEBUG) << "finalizing environment...";
	Py_Finalize();
}

std::string PythonEnvironment::getPythonClassNAme(const std::string & path) throw (std::invalid_argument) {
	size_t init = path.find_last_of("\\");
    if (init == string::npos) {
        init = path.find_last_of("/");
    }
	size_t end = path.find_last_of(".py");

	if (end > init) {
		std::string sub = path.substr(init + 1, (end - 3) - init);
		return sub;
	}
	else {
		throw(std::invalid_argument("incorrect path, \\ or .py missing"));
	}
}

std::string PythonEnvironment::getDirectoryPath(const std::string & path) throw (std::invalid_argument) {
	size_t init = path.find_last_of("\\");
    if (init == string::npos) {
        init = path.find_last_of("/");
    }

    if (init != string::npos) {
        std::string sub = path.substr(0, init);
        return sub;
    } else {
        throw(std::invalid_argument("incorrect path, \\ or .py missing"));
    }
}

object PythonEnvironment::executeStaticMethod(const std::string & type, const std::string & method) {
    try {
        std::string path = PluginFileLoader::GetInstance()->getPath(type);
        std::string dir = getDirectoryPath(path);
        if (loadedFiles.find(dir) == loadedFiles.end()) {
            LOG(DEBUG) << "adding path: " << dir;
            addImportPath(dir);
            loadedFiles.insert(dir);
        }

        object importObj;
        std::string importName = getPythonClassNAme(path);
        auto it = loadedClasses.find(importName);
        if (it == loadedClasses.end()) {
            LOG(DEBUG) << "importing file: " << importName;
            importObj = import(str(importName));
            loadedClasses.insert(std::make_pair(importName, importObj));
        }
        else {
            importObj = it->second;
        }

        auto temp = importObj.attr(str(type));
        return temp.attr(str(method))();
    } catch (error_already_set) {
        PyObject *ptype, *pvalue, *ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);
        char * cs = PyString_AsString(pvalue);

        std::string msg = "unknow";
        if (cs) {
            msg = std::string(cs);
        }
        throw(std::invalid_argument("error at python environment " + msg));
    }
}
