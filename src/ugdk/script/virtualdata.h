
#ifndef UGDK_SCRIPT_VIRTUALDATA_H_
#define UGDK_SCRIPT_VIRTUALDATA_H_

#include <vector>
#include <string>

namespace ugdk {

namespace script {

class VirtualObj;
class VirtualType;

/// Abstract class representing virtual script data.
class VirtualData {

  public:

    typedef std::tr1::shared_ptr<VirtualData> Ptr;

    virtual ~VirtualData() {}

    /// Tries to unwrap the data contained in this object using the given type.
    virtual void* Unwrap(const VirtualType& type) = 0;

    /// Tries to wrap the given data with the given type into this object.
    virtual void Wrap(void* data, const VirtualType& type) = 0;

	/// Tries to execute ourselves as a function in a script language,
    /// passing the given arguments and returning the result.
	virtual Ptr Execute(std::vector<const VirtualObj&> args);

	/// Tries to get a attribute with the given name from this object.
	virtual Ptr GetAttr(const std::string attr_name);

  protected:

    VirtualData() {}

};

} /* namespace script */

} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALDATA_H_ */
