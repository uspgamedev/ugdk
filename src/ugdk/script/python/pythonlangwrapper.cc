#include <Python.h>
#include <ugdk/script/python/pythonlangwrapper.h>
#include <ugdk/script/python/pythonvirtualdata.h>
#include <ugdk/script/virtualobj.h>


namespace ugdk {
namespace script {
namespace python {

VirtualData::Ptr WrapData(void* data, const VirtualType& type) {
	/*
	PyObject *obj;
    swig_type_info* typeInfo = <PEGA DO VirtualType>;
    obj = SWIG_NewInstanceObj(data, typeInfo, 1);

    return obj;
    */
    return VirtualData::Ptr(); //TODO
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

}
}
}
