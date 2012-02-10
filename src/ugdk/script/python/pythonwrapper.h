#ifndef UGDK_SCRIPT_PYTHON_PYTHONWRAPPER_H_
#define UGDK_SCRIPT_PYTHON_PYTHONWRAPPER_H_

#include <vector>
#include <ugdk/script/langwrapper.h>

namespace ugdk {
namespace script {

class VirtualObj;

namespace python {

typedef void (*PyInitFunction)(void);

struct Module {
    Module(const std::string& name, PyInitFunction init_func) :
        name_(name),
        init_func_(init_func) {}
    std::string     name_;
    PyInitFunction   init_func_;
};

class PythonWrapper : public LangWrapper {
  public:
	PythonWrapper() : LangWrapper("py", LANG(Python)) {}
	virtual ~PythonWrapper() {}

    virtual VirtualData::Ptr NewData();

	virtual void ExecuteCode(const std::string& code);
	
	virtual VirtualObj LoadModule(const std::string& name);

	/// Initializes the LangWrapper (that is, the language's API. Returns bool telling if (true=) no problems occured.
	virtual bool Initialize();
	/// Finalizes the LangWrapper, finalizing any language specific stuff.
	virtual void Finalize();

    bool RegisterModule(const std::string& moduleName, PyInitFunction init );

    void PrintPythonExceptionDetails();

  private:
    std::vector<Module> modules_;
};

}
}
}

#endif /* UGDK_SCRIPT_PYTHON_PYTHONWRAPPER_H_ */
