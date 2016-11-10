#pragma once

#pragma warning( disable : 4290 )

#define PLUGIN_FOLDER "pythonPlugins"

#include <windows.h>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <wchar.h>
#include <locale>
#include <codecvt>
#include <unordered_map>
#include <stdexcept>

//Qt
#include <QDirIterator>

//boost
#include <boost/algorithm/string.hpp>

//lib
#include <easylogging++.h>

//local
#include "util/Utils.h"

#include "evocodercore_global.h"

class PLUGINFILELOADER_EXPORT PluginFileLoader
{
public:
	static PluginFileLoader* GetInstance() {
		if (!m_pInstance)
			m_pInstance = new PluginFileLoader();
		return m_pInstance;
	}

	static void freeCommandInterface() {
		if (m_pInstance) {
			delete m_pInstance;
		}
	}

    static void setPluginDir(const std::string & pluginDir);

	virtual ~PluginFileLoader();

	std::string getPath(std::string name) throw (std::invalid_argument);
    std::string getSuperClass(std::string name) throw (std::invalid_argument);
    std::vector<std::string> getAllNames();

	void toText();
protected:
    static std::string pluginDir;

	std::unordered_map<std::string, std::string> namePathMap;
    std::unordered_map<std::string, std::string> nameSuperclassMap;

	bool checkDirectory(string path, string mask);
    void checkFile(std::string path);

private:
	PluginFileLoader();
    PluginFileLoader(const PluginFileLoader&) {}
    PluginFileLoader& operator=(const PluginFileLoader& com) {return PluginFileLoader(com);}

	static PluginFileLoader *m_pInstance;
};

