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

	virtual VirtualObj LoadModule(std::string name) = 0;

	/// Initializes the LangWrapper (that is, the language's API. Returns bool telling if (true=) no problems occured.
	virtual bool Initialize() = 0;
	/// Finalizes the LangWrapper, finalizing any language specific stuff.
	virtual void Finalize() = 0;

	virtual const LangID langID () = 0;

};

}
}

#endif /* UGDK_SCRIPT_LANGWRAPPER_H_ */
