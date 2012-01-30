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
	PyObject* arglist = PyTuple_New(args.size()); //new ref
	//PyTuple_SetItem(tuple, index(int), object) -> sets the element of the given index within the given tuple as the object passed
	// OBS ==> steals the reference to object
	for (int i = 0; i < args.size(); i++) {
		//check ref count of objects passed to Tuple_SetItem (since it steals)
		//check appropriate type of args[i]
		PyTuple_SetItem(arglist, i, args[i]->py_data_); //this is probably wrong...
	}
}

/// Tries to get a attribute with the given name from this object.
Ptr PythonVirtualData::GetAttribute(const std::string attr_name) {
}

}
}
}
