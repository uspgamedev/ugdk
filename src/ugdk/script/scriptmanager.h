#ifndef SCRIPTMANAGER_H_
#define SCRIPTMANAGER_H_

#include <string>
#include <map>

namespace ugdk {

namespace script {

class ScriptLangWrapper;
class VirtualObj;

class ScriptManager {
public:
	static ScriptManager* ref() { return ref_ ? ref_ : ref_ = new ScriptManager; }
	~ScriptManager() { ref_ = NULL; }

	bool Initialize();

	void Register(std::string name, ScriptLangWrapper* wrapper);
	ScriptLangWrapper* GetWrapper(std::string name);

	VirtualObj* LoadModule(std::string filepath);

private:
	static ScriptManager* ref_;
	ScriptManager();

	typedef std::map<std::string, ScriptLangWrapper*> WrapperMap;
	WrapperMap wrappers_;
};

}
}

#endif /* SCRIPTMANAGER_H_ */
