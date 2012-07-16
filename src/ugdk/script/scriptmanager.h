#ifndef UGDK_SCRIPT_SCRIPTMANAGER_H_
#define UGDK_SCRIPT_SCRIPTMANAGER_H_

#include <string>
#include <map>

#include <ugdk/script.h>

namespace ugdk {

namespace script {

#define SCRIPT_MANAGER() (ugdk::script::ScriptManager::ref())

class ScriptManager {
public:
	static ScriptManager* ref() {
	    return ref_ ? ref_ : ref_ = new ScriptManager;
	}
	~ScriptManager() { ref_ = NULL; }

	bool Initialize();
	void Finalize();

	void Register(const std::string& name, LangWrapper* wrapper);
	LangWrapper* GetWrapper(const std::string& name);

	void ExecuteCode(const std::string& language, const std::string& code);

	VirtualObj LoadModule(const std::string& filepath);
	
    bool CheckIfFileExists(const std::string& filepath);
    
    /// Converts "folder/subfolder/file" (without extension) style paths to "folder.subfolder.file"
    std::string ConvertPathToDottedNotation(const std::string& path);
    /// Converts dotted notation strings ("folder.subfolder.file") to "folder/subfolder/file".
    std::string ConvertDottedNotationToPath(const std::string& dotted);

private:
	static ScriptManager* ref_;
	ScriptManager();

	typedef std::map<std::string, LangWrapper*> WrapperMap;
	WrapperMap wrappers_;
};

}
}

#endif /* UGDK_SCRIPT_SCRIPTMANAGER_H_ */
