#include <Python.h>

#include <ugdk/script/languages/python/pythonwrapper.h>

#include <string>
#include <cstdlib>

#include <ugdk/portable/tr1.h>
#include FROM_TR1(memory)

#include <ugdk/config/config.h>
#include <ugdk/script/languages/python/pythondata.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/languages/python/swigpyrun.h>
#include <ugdk/script/languages/python/modules.h>
#include <ugdk/util/pathmanager.h>   // Two includes just so that we can use the engine's PathManager in a
#include <ugdk/base/engine.h>        // single line of code here. Not nice. =(

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
        fprintf(stderr, "[Python] Error loading module: '%s' (python exception details below)\n", dotted_name.c_str());
        PrintPythonExceptionDetails();
        return VirtualObj();
    }
#ifdef DEBUG
    printf("[Python] Loaded module '%s'.\n", dotted_name.c_str());
#endif
    VirtualData::Ptr vdata( new PythonData(this, module, true) ); //PythonData takes care of the ref.
    return VirtualObj(vdata);
}

/// Initializes the LangWrapper (that is, the language's API. Returns bool telling if (true=) no problems occured.
bool PythonWrapper::Initialize() {
#ifdef WIN32
    Py_NoSiteFlag = 1;
#endif
    Py_Initialize();
    
    char pathstr[5];
    strncpy(pathstr, "path", 4);
    pathstr[4] = '\0';
    PyObject *path = PySys_GetObject(pathstr);
    PyList_Append(path, PyString_FromString(PATH_MANAGER()->ResolvePath("scripts/").c_str()));
#ifdef UGDK_INSTALL_LOCATION
    PyList_Append(path, PyString_FromString(UGDK_INSTALL_LOCATION "/" UGDK_BIGVERSION "/python"));
#endif
    const char* ugdk_dir = getenv("UGDK_DIR");
    if(ugdk_dir) {
        std::string fullpath = ugdk_dir;
        fullpath += "/" UGDK_BIGVERSION "/python";
        PyList_Append(path, PyString_FromString(fullpath.c_str()));
    }

    RegisterModules(this);

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
    if(PyErr_Occurred() == NULL) {
        puts("No Exception.");
        return;
    }
    PyObject *temp, *exc_typ, *exc_val, *exc_tb;
    PyErr_Fetch(&exc_typ,&exc_val,&exc_tb);
    PyErr_NormalizeException(&exc_typ,&exc_val,&exc_tb);

    temp = PyObject_GetAttrString(exc_typ, "__name__");
    if (temp != NULL) {
        fprintf(stderr, "%s: ", PyString_AsString(temp));
        Py_DECREF(temp);
    }
    Py_DECREF(exc_typ);

    if(exc_val != NULL) {
        temp = PyObject_Str(exc_val);
        if (temp != NULL) {
            fprintf(stderr, "%s", PyString_AsString(temp));
            Py_DECREF(temp);
        }
        Py_DECREF(exc_val);
    }

    fprintf(stderr, "\n");
    if(exc_tb == NULL) return;
    
    PyObject *pName = PyString_FromString("traceback");
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    
    if(pModule == NULL) return;

    PyObject *pFunc = PyObject_GetAttrString(pModule, "format_tb");
    
    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject *pArgs = PyTuple_New(1);
        PyTuple_SetItem(pArgs, 0, exc_tb);

        PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
            Py_ssize_t len = PyList_Size(pValue);
            PyObject *t;
            for (Py_ssize_t i = 0; i < len; i++) {
                PyObject *tt = PyList_GetItem(pValue,i);
                t = Py_BuildValue("(O)",tt);
                
                char *buffer;
                if(!PyArg_ParseTuple(t,"s",&buffer)) break;

                fputs(buffer, stderr);
            }
        }
        Py_DECREF(pValue);
        Py_DECREF(pArgs);
    }
    Py_DECREF(pFunc);

    Py_DECREF(pModule);
}

}
}
}
