#include <ugdk/util/pathmanager.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/langwrapper.h>
#include <ugdk/script/virtualobj.h>

namespace ugdk {
namespace script {

using namespace std;

ScriptManager* ScriptManager::ref_ = NULL;

ScriptManager::ScriptManager() {
	// TODO Auto-generated constructor stub

}

bool ScriptManager::Initialize() {
	bool is_ok = true;

	WrapperMap::iterator it = wrappers_.begin();
	while (it != wrappers_.end()) {
		LangWrapper* wrap = it->second;
		is_ok = is_ok && wrap->Initialize();
		++it;
	}

	return is_ok;
}

void ScriptManager::Finalize() {
	WrapperMap::iterator it = wrappers_.begin();
	while (it != wrappers_.end()) {
		LangWrapper* wrap = it->second;
		wrap->Finalize()
		++it;
	}
}

void ScriptManager::Register(string name, LangWrapper* wrapper) {
	if (wrappers_.count(name))	return;
	wrappers_[name] = wrapper;
}

LangWrapper* ScriptManager::GetWrapper(string name) {
	if (!wrappers_.count(name))	return NULL;
	return wrappers_[name];
}

VirtualObj* ScriptManager::LoadModule(string script) {
	string filepath = PATH_MANAGER()->ResolvePath("scripts/" + script);

	WrapperMap::iterator it = wrappers_.begin();
	while (it != wrappers_.end()) {
		LangWrapper* wrap = it->second;
		if (wrap->CheckIfModuleExists(filepath)) {
			return wrap->LoadModule(filepath);
		}
		++it;
	}
}

}
}
