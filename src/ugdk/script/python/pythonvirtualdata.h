#ifndef UGDK_SCRIPT_PYTHON_VIRTUALDATA_H_
#define UGDK_SCRIPT_PYTHON_VIRTUALDATA_H_

#include <Python.h>
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
	PythonVirtualData(PyObject* data_object, bool owns_ref) : VirtualData(), 
		own_ref_(owns_ref), py_data_(data_object) {}
		
    virtual ~PythonVirtualData() {
    	if (own_ref_)
    		Py_XDECREF(py_data_);
    }

    /// Tries to unwrap the data contained in this object using the given type.
    virtual void* Unwrap(const VirtualType& type) const;

    /// Tries to wrap the given data with the given type into this object.
    virtual void Wrap(void* data, const VirtualType& type);
    virtual void Wrap(const char* str);
    virtual void Wrap(bool boolean);
    virtual void Wrap(int number);
    virtual void Wrap(double number);

	virtual LangWrapper* wrapper () const;

	/// Tries to execute ourselves as a function in a script language,
    /// passing thee given arguments and returning the result.
	virtual Ptr Execute(std::vector<Ptr> args);

	/// Tries to get a attribute with the given name from this object.
	virtual Ptr GetAttribute(const std::string attr_name);

    virtual void SetAttribute(Ptr attr_name, Ptr value);

  protected:
	bool own_ref_; //if true, we own a ref to our PyObject* (py_data_), so we need to DECREF it in due time.
	PyObject* py_data_;
};

}
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_PYTHON_VIRTUALDATA_H_ */
