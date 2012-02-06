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

	void Register(std::string name, LangWrapper* wrapper);
	LangWrapper* GetWrapper(std::string name);

	VirtualObj LoadModule(std::string filepath);

private:
	static ScriptManager* ref_;
	ScriptManager();

	typedef std::map<std::string, LangWrapper*> WrapperMap;
	WrapperMap wrappers_;
};

}
}

#endif /* UGDK_SCRIPT_SCRIPTMANAGER_H_ */
