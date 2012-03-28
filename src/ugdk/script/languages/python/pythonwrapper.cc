
#include <Python.h>
#include <ugdk/script/languages/python/pythonwrapper.h>
#include <ugdk/script/languages/python/pythondata.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/languages/python/swigpyrun.h>
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
	VirtualData::Ptr vdata( new PythonData(this, NULL, false) ); 
	return vdata;
}

void PythonWrapper::ExecuteCode(const std::string& code) {
	PyRun_SimpleString(code.c_str());
}

VirtualObj PythonWrapper::LoadModule(const std::string& name) {
    std::string dotted_name =
        SCRIPT_MANAGER()->ConvertPathToDottedNotation(name);
	PyObject* module = PyImport_ImportModule(dotted_name.c_str()); //new ref
    if (module == NULL) {
        printf("[Python] Error loading module: \"%s\" (python exception details below)\n", dotted_name.c_str());
        PrintPythonExceptionDetails();
        return VirtualObj();
    }
    printf("[Python] Loading module: %s\n", dotted_name.c_str());
	VirtualData::Ptr vdata( new PythonData(this, module, true) ); //PythonData takes care of the ref.
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

    std::vector<PythonModule>::iterator it;
    for (it = modules_.begin(); it != modules_.end(); ++it) {
        (*it->init_func())();
    }
	return true;
}

/// Finalizes the LangWrapper, finalizing any language specific stuff.
void PythonWrapper::Finalize() {
	Py_Finalize();
}

void PythonWrapper::PrintPythonExceptionDetails() {
    PyObject *exc_type=NULL, *exc_value=NULL, *exc_tb=NULL, *arglist=NULL,
             *traceback=NULL, *format=NULL, *errlist=NULL, *errstr=NULL;

    do {
        PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
        /*value and tb can be null (no exception) :: we own ref to all of them*/

        PyObject* arglist = PyTuple_New(3); //new ref
	    if (arglist == NULL) {
            printf("[Python] Error 1... Can't print exception details... >_<\n");
            break;
        }
        PyTuple_SetItem(arglist, 0, exc_type);   // PyTuple_SetItem
        PyTuple_SetItem(arglist, 1, exc_value);  // steals the reference
        PyTuple_SetItem(arglist, 2, exc_tb);	 // to the given item

        /*It's kinda ugly to do this, but if we do not use traceback.format_exception
          I would have a LOT of work to do here =P*/
        traceback = PyImport_ImportModule("traceback");
        if (traceback == NULL) {
            printf("[Python] Error 2... Can't print exception details... >_<\n");
            break;
        }

        format = PyObject_GetAttrString(traceback, "format_exception"); //return is new ref
        if (format == NULL) {
            printf("[Python] Error 3... Can't print exception details... >_<\n");
            break;
        }

        errlist = PyObject_CallObject(format, arglist); //return is new ref
        if (errlist == NULL) {
            printf("[Python] Error 4... Can't print exception details... >_<\n");
            break;
        }

        errstr = PyString_FromString("[Python] Exception Details:\n");
        PyObject* errstr_part;
        Py_ssize_t size = PyList_Size(errlist);
        for (Py_ssize_t i=0; i<size; i++) {
            errstr_part = PyList_GetItem(errlist, i); //borrowed reference
            PyString_Concat(&errstr, errstr_part);
        }

        if (errstr == NULL) {
            printf("[Python] Error 5... Can't print exception details... >_<\n");
            break;
        }

        char* message = PyString_AsString(errstr);
        printf("%s\n", message);

    } while (0);

    Py_XDECREF(arglist);
    Py_XDECREF(traceback);
    Py_XDECREF(format);
    Py_XDECREF(errlist);
    Py_XDECREF(errstr);
    
    /*///////////////////////////////////////
    // what we do above in C++ is basically the following in Python:
    PyRun_SimpleString("import sys, traceback");
    PyRun_SimpleString("exc = sys.exc_info()");
    PyRun_SimpleString("tb = traceback.format_exception(exc[0], exc[1], exc[2])");
    PyRun_SimpleString("for s in tb:    print s");*/
}

}
}
}
