#ifndef UGDK_SCRIPT_PYTHON_LANGWRAPPER_H_
#define UGDK_SCRIPT_PYTHON_LANGWRAPPER_H_

#include <ugdk/script/python/pythonvirtualdata.h>
#include <ugdk/script/langwrapper.h>

namespace ugdk {
namespace script {

class VirtualObj;

namespace python {

class PythonLangWrapper : public LangWrapper {
  public:
	PythonLangWrapper() : LangWrapper(".py", LANG(Python)) {}
	virtual ~PythonLangWrapper() {}

	virtual VirtualData::Ptr WrapData(void* data, const VirtualType& type);

	virtual VirtualObj LoadModule(std::string name);

	/// Initializes the LangWrapper (that is, the language's API. Returns bool telling if (true=) no problems occured.
	virtual bool Initialize();
	/// Finalizes the LangWrapper, finalizing any language specific stuff.
	virtual void Finalize();

    bool RegisterModule(std::string moduleName, void (*initFunction)(void) );
};

}
}
}

#endif /* UGDK_SCRIPT_PYTHON_LANGWRAPPER_H_ */
