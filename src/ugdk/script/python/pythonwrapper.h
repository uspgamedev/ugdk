#ifndef UGDK_SCRIPT_PYTHON_PYTHONWRAPPER_H_
#define UGDK_SCRIPT_PYTHON_PYTHONWRAPPER_H_

#include <ugdk/script/python/pythondata.h>
#include <ugdk/script/langwrapper.h>

namespace ugdk {
namespace script {

class VirtualObj;

namespace python {

class PythonWrapper : public LangWrapper {
  public:
	PythonWrapper() : LangWrapper(".py", LANG(Python)) {}
	virtual ~PythonWrapper() {}

    virtual VirtualData::Ptr NewData();

	virtual VirtualObj LoadModule(const std::string& name);

	/// Initializes the LangWrapper (that is, the language's API. Returns bool telling if (true=) no problems occured.
	virtual bool Initialize();
	/// Finalizes the LangWrapper, finalizing any language specific stuff.
	virtual void Finalize();

    bool RegisterModule(std::string moduleName, void (*initFunction)(void) );
};

}
}
}

#endif /* UGDK_SCRIPT_PYTHON_PYTHONWRAPPER_H_ */
