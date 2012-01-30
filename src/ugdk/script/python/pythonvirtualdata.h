#ifndef UGDK_SCRIPT_PYTHON_VIRTUALDATA_H_
#define UGDK_SCRIPT_PYTHON_VIRTUALDATA_H_

#include <vector>
#include <string>
#include <ugdk/script/virtualdata.h>

namespace ugdk {
namespace script {

class VirtualObj;
class VirtualType;

namespace python {

class PythonVirtualData : public VirtualData {
  public:
	PythonVirtualData() : VirtualData() {}
    virtual ~PythonVirtualData() {}

    /// Tries to unwrap the data contained in this object using the given type.
    virtual void* Unwrap(const VirtualType& type);

    /// Tries to wrap the given data with the given type into this object.
    virtual void Wrap(void* data, const VirtualType& type);

	virtual LangWrapper* wrapper ();

	/// Tries to execute ourselves as a function in a script language,
    /// passing thee given arguments and returning the result.
	virtual Ptr Execute(std::vector<const VirtualObj&> args);

	/// Tries to get a attribute with the given name from this object.
	virtual Ptr GetAttr(const std::string attr_name);

};

}
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_PYTHON_VIRTUALDATA_H_ */
