#include <ugdk/script/scriptmanager.h>

#include <cstdio>
#include <algorithm>

#include <ugdk/base/engine.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/script/langwrapper.h>
#include <ugdk/script/virtualobj.h>

namespace ugdk {
namespace script {

using std::string;

ScriptManager* ScriptManager::ref_ = nullptr;

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
        wrap->Finalize();
        delete wrap;
        ++it;
    }
    wrappers_.clear();
}

void ScriptManager::Register(LangWrapper* wrapper) {
    if (wrappers_.count(wrapper->lang_name())) return;
    wrappers_[wrapper->lang_name()] = wrapper;
}

LangWrapper* ScriptManager::GetWrapper(const string& name) {
    if (!wrappers_.count(name))	return nullptr;
    return wrappers_[name];
}

void ScriptManager::ExecuteCode(const string& language, const string& code) {
    if (!wrappers_.count(language)) return;
    
    wrappers_[language]->ExecuteCode(code);
}


VirtualObj ScriptManager::LoadModule(const string& script) {
    string filepath = PATH_MANAGER()->ResolvePath(
        "scripts/" + ConvertDottedNotationToPath(script)
    );

    WrapperMap::iterator it = wrappers_.begin();
    while (it != wrappers_.end()) {
        LangWrapper* wrap = it->second;
        if ( CheckIfFileExists(filepath + "." + wrap->file_extension()) ) {
            return wrap->LoadModule( script );
        }
        ++it;
    }
    fprintf(stderr, "Uncapable of loading module (path notation) \"%s\".\n", filepath.c_str() );
    return VirtualObj();
}

bool ScriptManager::CheckIfFileExists(const string& filepath) {
    FILE* file = fopen(filepath.c_str(), "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

/// Converts "folder/subfolder/file" (without extension) style paths to "folder.subfolder.file"
std::string ScriptManager::ConvertPathToDottedNotation(const std::string& path) {
    string dotted( path );
    replace(dotted.begin(), dotted.end(), '/', '.');
    return dotted;
}

/// Converts dotted notation strings ("folder.subfolder.file") to "folder/subfolder/file".
std::string ScriptManager::ConvertDottedNotationToPath(const std::string& dotted) {
    string path( dotted );
    replace(path.begin(), path.end(), '.', '/');
    return path;
}

}
}
