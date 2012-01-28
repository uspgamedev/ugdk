#include <ugdk/script/python/pythonvirtualdata.h>

namespace ugdk {
namespace script {
namespace python {

void* PythonVirtualData::Unwrap(const VirtualType& type) {
}

/// Tries to wrap the given data with the given type into this object.
void PythonVirtualData::Wrap(void* data, const VirtualType& type) {
}

/// Tries to execute ourselves as a function in a script language,
/// passing the given arguments and returning the result.
Ptr PythonVirtualData::Execute(std::vector<const VirtualObj&> args) {
}

/// Tries to get a attribute with the given name from this object.
Ptr PythonVirtualData::GetAttr(const std::string attr_name) {
}

}
}
}
