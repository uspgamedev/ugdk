#ifndef UGDK_SCRIPT_LANGWRAPPER_H_
#define UGDK_SCRIPT_LANGWRAPPER_H_

#include <string>
#include <cstdio>

#include <ugdk/script/defs.h>
#include <ugdk/script/type.h>

namespace ugdk {
namespace script {

class VirtualObj;

class LangWrapper {
  public:
	LangWrapper();
	virtual ~LangWrapper();

	virtual std::string script_file_extension() = 0;

	bool CheckIfModuleExists(std::string filepath) {
		FILE* file = fopen((filepath + script_file_extension()).c_str(), "r");
		if (file) {
			fclose(file);
			return true;
		}
		return false;
	}

	virtual VirtualObjPtr LoadModule(std::string name) = 0;

	const LangID langID () { return LANG(Lua); }

};

}
}

#endif /* UGDK_SCRIPT_LANGWRAPPER_H_ */
