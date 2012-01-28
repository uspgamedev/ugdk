#ifndef UGDK_SCRIPT_PYTHON_LANGWRAPPER_H_
#define UGDK_SCRIPT_PYTHON_LANGWRAPPER_H_

#include <ugdk/script/langwrapper.h>

namespace ugdk {
namespace script {

class VirtualObj;

namespace python {

class PythonLangWrapper : public LangWrapper {
  public:
	PythonLangWrapper() : LangWrapper() {}
	virtual ~PythonLangWrapper() {}

	virtual std::string script_file_extension() { return ".py" };

	virtual VirtualObj LoadModule(std::string name);

	/// Initializes the LangWrapper (that is, the language's API. Returns bool telling if (true=) no problems occured.
	virtual bool Initialize();
	/// Finalizes the LangWrapper, finalizing any language specific stuff.
	virtual void Finalize();

	const LangID langID () { return LANG(Python); }

};

}
}
}

#endif /* UGDK_SCRIPT_PYTHON_LANGWRAPPER_H_ */
