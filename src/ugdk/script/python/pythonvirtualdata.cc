#include <ugdk/script/python/pythonvirtualdata.h>
#include <ugdk/script/scriptmanager.h>

namespace ugdk {
namespace script {
namespace python {

void* PythonVirtualData::Unwrap(const VirtualType& type) {
}

/// Tries to wrap the given data with the given type into this object.
void PythonVirtualData::Wrap(void* data, const VirtualType& type) {
}

LangWrapper* PythonVirtualData::wrapper () {
	ScriptManager::ref()->GetWrapper("Python");
}

/// Tries to execute ourselves as a function in a script language,
/// passing the given arguments and returning the result.
Ptr PythonVirtualData::Execute(std::vector<Ptr> args) {
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
	for (int i = 0; i < args.size(); i++) {
		//check appropriate type of args[i]
		PythonVirtualData* py_arg = static_cast<PythonVirtualData*>(args[i].get());
		if (py_arg->own_ref_) {
			//this VData arg owns the ref to the Object it's pointing at. And since PyTuple_SetItem steals that reference...
			py_arg->own_ref_ = false;
		}
		else {
			//VData arg doesn't own its reference... We can't let PyTuple_SetItem steal a ref that isn't ours to begin with.
			Py_INCREF(py_arg->py_data_);
		}
		PyTuple_SetItem(arglist, i, py_arg->py_data_);
	}

	PyObject* result = PyObject_CallObject(py_data_, arglist); //return is new ref
	/*If result = NULL, CallObject failed (somehow), and due to the way we treat VirtualData in the system, 
	  this is the way to treat that.*/
	VirtualData::Ptr vdata( new PythonVirtualData(result, true) ); //PyVirtualData takes care of the ref.
	
	//Before returning, kill the argument tuple we created.
	Py_XDECREF(arglist);  //WARNING: in theory, this is correct. However in my testing (in the prototype) this cause some serious shit.
	
	return VirtualObj(vdata);
}

/// Tries to get a attribute with the given name from this object.
Ptr PythonVirtualData::GetAttribute(const std::string attr_name) {
	PyObject* attr;
	
	/*We give preference for getting a object's attributes. If it doesn't have a attribute
	  with the given name, check if he has a item with key = attr_name */
	if (PyObject_HasAttrString(py_data_, attr_name.c_str())) {
		attr = PyObject_GetAttrString(py_data_, attr_name.c_str()); //return is new ref
	}
	else {
		if (!PyMapping_Check(py_data_)) {
			//Object doesn't have attribute with the given name and can't have items...
			return VirtualData::Ptr();
		}
		if (!PyMapping_HasKeyString(py_data_, attr_name.c_str())) {
			//Object doesn't have attribute or item with the given name...
			return VirtualData::Ptr();
		}
		attr = PyMapping_GetItemString(py_data_, attr_name.c_str()); //return is new ref
	}
	/*If Py_GetAttrString or Py_GetItemString failed somehow, they will return null.
	  */
	
	VirtualData::Ptr vdata( new PythonVirtualData(attr, true) );
	return vdata;
}

}
}
}
