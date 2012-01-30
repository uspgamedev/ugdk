#include <Python.h>
#include <ugdk/script/python/pythonlangwrapper.h>
#include <ugdk/script/python/pythonvirtualdata.h>
#include <ugdk/script/virtualobj.h>


namespace ugdk {
namespace script {
namespace python {

VirtualObj PythonLangWrapper::LoadModule(std::string name) {
	PyObject* module = PyImport_ImportModule(name.c_str()); //new ref
	VirtualData::Ptr vdata( new PythonVirtualData(module, true) ); //PyVirtualData takes care of the ref.
	return VirtualObj(vdata);
}

/// Initializes the LangWrapper (that is, the language's API. Returns bool telling if (true=) no problems occured.
bool PythonLangWrapper::Initialize() {
	Py_Initialize();
}

/// Finalizes the LangWrapper, finalizing any language specific stuff.
void PythonLangWrapper::Finalize() {
	Py_Finalize();
}

}
}
}
