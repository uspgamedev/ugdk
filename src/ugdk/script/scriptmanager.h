#ifndef UGDK_SCRIPT_SCRIPTMANAGER_H_
#define UGDK_SCRIPT_SCRIPTMANAGER_H_

#include <string>
#include <map>

namespace ugdk {

namespace script {

class LangWrapper;
class VirtualObj;

class ScriptManager {
public:
	static ScriptManager* ref() { return ref_ ? ref_ : ref_ = new ScriptManager; }
	~ScriptManager() { ref_ = NULL; }

	bool Initialize();

	void Register(std::string name, LangWrapper* wrapper);
	LangWrapper* GetWrapper(std::string name);

	VirtualObj* LoadModule(std::string filepath);

private:
	static ScriptManager* ref_;
	ScriptManager();

	typedef std::map<std::string, LangWrapper*> WrapperMap;
	WrapperMap wrappers_;
};

}
}

#endif /* UGDK_SCRIPT_SCRIPTMANAGER_H_ */
