
#include <Python.h>
#include <ugdk/script/python/pythonwrapper.h>
#include <ugdk/script/python/pythondata.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/swig/swigpyrun.h>
#include <ugdk/util/pathmanager.h>   // Two includes just so that we can use the engine's PathManager in a
#include <ugdk/base/engine.h>        // single line of code here. Not nice. =(
#include <string>
#include <memory>
#include <cstdlib>

namespace ugdk {
namespace script {
namespace python {

using std::tr1::shared_ptr;

VirtualData::Ptr PythonWrapper::NewData() {
	VirtualData::Ptr vdata( new PythonData(NULL, false) ); 
	return vdata;
}

VirtualObj PythonWrapper::LoadModule(const std::string& name) {
	PyObject* module = PyImport_ImportModule(name.c_str()); //new ref
	if (module == NULL && PyErr_Occurred() != NULL) {
						
		PyErr_Print();
		return VirtualObj();
	}
	VirtualData::Ptr vdata( new PythonData(module, true) ); //PythonData takes care of the ref.
	return VirtualObj(vdata);
}

/// Initializes the LangWrapper (that is, the language's API. Returns bool telling if (true=) no problems occured.
bool PythonWrapper::Initialize() {
	Py_Initialize();
	//TODO: Fix sys.path with our paths...
    PyRun_SimpleString("import sys");
    std::string command = "sys.path.append(\"" + PATH_MANAGER()->ResolvePath("scripts/") + "\")";
    //std::string command = "sys.path.append(\"./\")";
    PyRun_SimpleString(command.c_str());
	return true;
}

/// Finalizes the LangWrapper, finalizing any language specific stuff.
void PythonWrapper::Finalize() {
	Py_Finalize();
}

bool PythonWrapper::RegisterModule(std::string moduleName, void (*initFunction)(void) ) {
    shared_ptr<char> str(new char(*(moduleName.c_str())), free);
    return PyImport_AppendInittab(str.get(), initFunction) == 0;
}

}
}
}
