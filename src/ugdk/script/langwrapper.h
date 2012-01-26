#ifndef UGDK_SCRIPT_LANGWRAPPER_H_
#define UGDK_SCRIPT_LANGWRAPPER_H_

#include <string>
#include <cstdio>

namespace ugdk {
namespace script {

class VirtualObj;

class LangWrapper {
  public:
	LangWrapper();
	virtual ~LangWrapper();

	virtual std::string script_file_extension();

	bool CheckIfModuleExists(std::string filepath) {
		FILE* file = fopen((filepath + script_file_extension()).c_str(), "r");
		if (file) {
			fclose(file);
			return true;
		}
		return false;
	}
	virtual VirtualObj* LoadModule(std::string name);
};

}
}

#endif /* UGDK_SCRIPT_LANGWRAPPER_H_ */
