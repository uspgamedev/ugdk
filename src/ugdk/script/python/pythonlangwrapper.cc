#include <Python.h>
#include <ugdk/script/python/pythonlangwrapper.h>
#include <ugdk/script/python/pythonvirtualdata.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/swig/swigpyrun.h>

#include <memory>
#include <cstdlib>

namespace ugdk {
namespace script {
namespace python {

using std::tr1::shared_ptr;

VirtualData::Ptr WrapData(void* data, const VirtualType& type) {
	PyObject *obj;
    obj = SWIG_NewInstanceObj(data, type.FromLang(LANG(Python)), 1);

	/*Apparently, the PyObject return by the SWIG conversion function is a new reference.
	  So our PyVData needs to handle it.*/
	VirtualData::Ptr vdata( new PythonVirtualData(obj, true) ); 
	return vdata;
}

VirtualObj PythonLangWrapper::LoadModule(std::string name) {
	PyObject* module = PyImport_ImportModule(name.c_str()); //new ref
	if (module == NULL && PyErr_Occurred() != NULL) {
						
		PyErr_Print();
		return VirtualObj();
	}
	VirtualData::Ptr vdata( new PythonVirtualData(module, true) ); //PyVirtualData takes care of the ref.
	return VirtualObj(vdata);
}

/// Initializes the LangWrapper (that is, the language's API. Returns bool telling if (true=) no problems occured.
bool PythonLangWrapper::Initialize() {
	Py_Initialize();
	//TODO: Fix sys.path with our paths...
	return true;
}

/// Finalizes the LangWrapper, finalizing any language specific stuff.
void PythonLangWrapper::Finalize() {
	Py_Finalize();
}

bool RegisterModule(std::string moduleName, void (*initFunction)(void) ) {
    shared_ptr<char*> str(new char(*(moduleName.c_str())), free);
    return PyImport_AppendInittab(str.get(), initFunction) == 0;
}

}
}
}
