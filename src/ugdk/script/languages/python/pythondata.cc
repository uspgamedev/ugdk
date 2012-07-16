#include <ugdk/script/languages/python/pythondata.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/languages/python/swigpyrun.h>

#include <memory>
#include <cstdlib>

namespace ugdk {
namespace script {
namespace python {

using std::tr1::shared_ptr;

void* PythonData::Unwrap(const VirtualType& type, bool disown) const {
	void* pointer;
	int res = SWIG_ConvertPtrAndOwn(py_data_, &pointer, type.FromLang(LANG(Python)), 
                                    disown ? SWIG_POINTER_DISOWN : 0, NULL);
	if (SWIG_IsOK(res))
	    return pointer;
	return NULL;
}
const char* PythonData::UnwrapString() const {
    if (py_data_ == NULL)   return NULL;
    return PyString_AsString(py_data_);
}
bool PythonData::UnwrapBoolean() const {
    return !!PyObject_IsTrue(py_data_); //HEEEEL YEAAAAH!!
}
int PythonData::UnwrapInteger() const {
    return static_cast<int>( PyInt_AsLong(py_data_) );
}
double PythonData::UnwrapNumber() const {
    return static_cast<double>( PyFloat_AsDouble(py_data_) );
}

/*************/
template <class T>
static T UnwrapSequence(PyObject* py_data_, PythonWrapper* wrapper_) {
    T seq = T();
    if (py_data_ == NULL)   return seq;
    if (!PySequence_Check(py_data_)) return seq;

    Py_ssize_t size = PySequence_Length(py_data_);
    PyObject* obj;
    PythonData* data;
    for (Py_ssize_t i = 0; i < size; i++) {
        obj = PySequence_GetItem(py_data_, i); //returns new ref, NULL on failure
        data = new PythonData(wrapper_, obj, true); //PythonData takes care of the new ref
        seq.push_back(  VirtualData::Ptr(data)  );
    }
	return seq;
}
/*************/

VirtualData::Vector PythonData::UnwrapVector() const {
    return UnwrapSequence<Vector>(py_data_, wrapper_);
}
VirtualData::List PythonData::UnwrapList() const {
    return UnwrapSequence<List>(py_data_, wrapper_);
}
VirtualData::Map PythonData::UnwrapMap() const {
    Map d = Map();
    if (py_data_ == NULL)   return d;
    if (!PyMapping_Check(py_data_)) return d;

    PyObject* items_func = PyObject_GetAttrString(py_data_, "items"); //new ref
    if (items_func == NULL) return d;
    PyObject* items = PyObject_CallObject(items_func, NULL);  //new ref
    Py_XDECREF(items_func);
    //PyObject* items = PyMapping_Items(py_data_); //items is new ref
    if (items == NULL)   return d;    

    Py_ssize_t size = PySequence_Length(items);
    PyObject *item, *key, *value;
    PythonData *key_data, *value_data;
    for (Py_ssize_t i = 0; i < size; i++) {
        item = PySequence_GetItem(items, i); //returns new ref, NULL on failure
        if (item == NULL)   continue;

        key = PySequence_GetItem(item, 0);  //key -> new ref
        value = PySequence_GetItem(item, 1);  //value -> new ref

        key_data = new PythonData(wrapper_, key, true); //PythonData takes care of the new ref
        value_data = new PythonData(wrapper_, value, true); //PythonData takes care of the new ref

        d[ VirtualData::Ptr(key_data) ] = VirtualData::Ptr(value_data);
        
        Py_XDECREF(item);
    }
    Py_XDECREF(items);
    
    return d;
}



/// Tries to wrap the given data with the given type into this object.
void PythonData::Wrap(void* data, const VirtualType& type) {
    if (py_data_ != NULL && own_ref_) {
        Py_DECREF(py_data_);
        py_data_ = NULL;
    }

    py_data_ = SWIG_NewInstanceObj(data, type.FromLang(LANG(Python)), 1);
    own_ref_ = true;
	/*Apparently, the PyObject return by the SWIG conversion function is a new reference.
	  So our PyVData needs to handle it.*/
}

void PythonData::WrapString(const char* str) {
    if (str == NULL)    return;
    if (py_data_ != NULL && own_ref_) {
        Py_DECREF(py_data_);
        py_data_ = NULL;
    }

    py_data_ = PyString_FromString(str);
    own_ref_ = true;
}

void PythonData::WrapBoolean(bool boolean) {
    py_data_ = PyBool_FromLong(static_cast<long>(boolean));
    own_ref_ = true;
}

void PythonData::WrapInteger(int number) {
    if (py_data_ != NULL && own_ref_) {
        Py_DECREF(py_data_);
        py_data_ = NULL;
    }
    
    py_data_ = PyInt_FromLong(static_cast<long>(number));
    own_ref_ = true;
}

void PythonData::WrapNumber(double number) {
    if (py_data_ != NULL && own_ref_) {
        Py_DECREF(py_data_);
        py_data_ = NULL;
    }

    py_data_ = PyFloat_FromDouble(number);
    own_ref_ = true;
}

/// Tries to execute ourselves as a function in a script language,
/// passing the given arguments and returning the result.
VirtualData::Ptr PythonData::Execute(const std::vector<Ptr>& args) {
	if (!PyCallable_Check(py_data_)) {
		//The object we contain is not callable - cannot execute it.
		return VirtualData::Ptr();
	}

	PyObject* arglist = PyTuple_New(args.size()); //new ref
	if (arglist == NULL) {
		/*Some error occured... Py/C API doesn't specify what might cause the failure here...
		   Most likely out of memory? */
		return VirtualData::Ptr();
	}

	//PyTuple_SetItem(tuple, index(int), object) -> sets the element of the given index within the given tuple as the object passed
	// OBS.: steals the reference to object;  returns 0 on success
	for (unsigned int i = 0; i < args.size(); i++) {
		//check appropriate type of args[i]
		PythonData* py_arg = static_cast<PythonData*>(args[i].get());
		
		/* We INCREF the refcount of all args passed since PyTuple_SetItem steals the ref, and when it is
		   deleted when this method finishes, it will DECREF this ref.
		   Since VObjs use shared_ptr to hold VDatas, the system only has 1 VData with a unique py_data_, and the others
		   are refs to it. Therefore what we have here are not copies, we can't let the ref of the args' py_data_ go away!
		   =P
		   (in other words, if we do not do this, the py_data_ passed as arg may have a bad refcount and be deleted
		    and so if we try to use it after this some shit will happen)*/
		Py_INCREF(py_arg->py_data_);
		
		PyTuple_SetItem(arglist, i, py_arg->py_data_);
	}

	PyObject* result = PyObject_CallObject(py_data_, arglist); //return is new ref
	/*If result = NULL, CallObject failed (somehow)*/
    if (result == NULL) {
        printf("[Python] Erro executing callable object (python exception details below)\n");
        wrapper_->PrintPythonExceptionDetails();
        return VirtualData::Ptr();
    }

    //WAAARPPER!!!
	VirtualData::Ptr vdata( new PythonData(wrapper_, result, true) ); //PyVirtualData takes care of the ref.
	
	//Before returning, kill the argument tuple we created.
	Py_XDECREF(arglist);  //WARNING: in theory, this is correct. However in my testing (in the prototype) this cause some serious shit.
	
	return vdata;
}

/// Tries to get a attribute with the given name from this object.
VirtualData::Ptr PythonData::GetAttribute(Ptr key) {
    PythonData* vkey = static_cast<PythonData*>(key.get());
    PyObject* key_data = vkey->py_data_;
	PyObject* attr;
	
	/*We give preference for getting a object's attributes. If it doesn't have a attribute
	  with the given name, check if he has a item with key = attr_name */
	if (PyObject_HasAttr(py_data_, key_data)) {
		attr = PyObject_GetAttr(py_data_, key_data); //return is new ref

        if (PyCallable_Check(attr) && PyObject_HasAttrString(attr, "__func__") ) {
            /*return attr.__func__, so that we always return a function or unbinded method (without self)*/
            PyObject* original = attr;

            attr = PyObject_GetAttrString(original, "__func__"); //another new ref
            Py_XDECREF(original);
        }
	}
	else {
		if (!PyMapping_Check(py_data_)) {
			//Object doesn't have attribute with the given name and can't have items...
			return VirtualData::Ptr();
		}
		if (!PyMapping_HasKey(py_data_, key_data) ) {
			//Object doesn't have attribute or item with the given name...
			return VirtualData::Ptr();
		}
		attr = PyObject_GetItem(py_data_, key_data); //return is new ref
	}
	/*If Py_GetAttr or Py_GetItem failed somehow, they will return null.*/
	
	VirtualData::Ptr vdata( new PythonData(wrapper_, attr, true) );
	return vdata;
}

VirtualData::Ptr PythonData::SetAttribute(Ptr key, Ptr value) {
    PythonData* vkey = static_cast<PythonData*>(key.get());
    PyObject* key_data = vkey->py_data_;
    PythonData* object = static_cast<PythonData*>(value.get());
    PyObject* object_data = object->py_data_;

    /*First we try checking if our object has a attribute or string with the given
      name to override it...*/
    if (PyObject_HasAttr(py_data_, key_data) || !PyMapping_Check(py_data_) ) {
        PyObject_SetAttr(py_data_, key_data, object_data);
    }
    else { /*We do not have a attribute with given key and we are a mapping, so...*/
        PyObject_SetItem(py_data_, key_data, object_data);
    }
    return value;
}

}
}
}
