
#ifndef UGDK_SCRIPT_VIRTUALDATA_H_
#define UGDK_SCRIPT_VIRTUALDATA_H_

#include <vector>
#include <string>

#include <ugdk/script/type.h>

namespace ugdk {

namespace script {

class VirtualObj;

/// Abstract class representing virtual script data.
class VirtualData {

  public:

    typedef std::tr1::shared_ptr<VirtualData> Ptr;

    virtual ~VirtualData() {}

    /// Tries to unwrap the data contained in this object using the given type.
    virtual void* Unwrap(const VirtualType& type) = 0;

    /// Tries to wrap the given data with the given type.
    /** Returns a new VirtualData::Ptr with the data wrapped upon success.
     ** The VirtualData object used to call this method DOES NOT get updated
     ** with the new wrapped data.
     ** @param data - a void pointer to the data being wrapped.
     ** @param type - a virtual type got from TypeRegistry<T>::type(), where
     **               T is the original declarated type of the data.
     ** @return A shared pointer to the wrapped data.
     ** @see TypeRegistry
     */
    virtual Ptr Wrap(void* data, const VirtualType& type) = 0;

    virtual LangWrapper* wrapper () = 0;

	/// Tries to execute ourselves as a function in a script language,
    /// passing the given arguments and returning the result.
	virtual Ptr Execute(std::vector<VirtualObj> args) = 0;

	/// Tries to get a attribute with the given name from this object.
	virtual Ptr GetAttr(const std::string attr_name) = 0;

  protected:

    VirtualData() {}

};

} /* namespace script */

} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALDATA_H_ */
