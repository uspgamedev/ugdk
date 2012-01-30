
#ifndef UGDK_SCRIPT_VIRTUALDATA_H_
#define UGDK_SCRIPT_VIRTUALDATA_H_

#include <vector>
#include <string>
#include <memory>

#include <ugdk/script/type.h>

namespace ugdk {

namespace script {

class VirtualObj;

/// Abstract class representing virtual script data.
class VirtualData : public std::tr1::enable_shared_from_this<VirtualData> {

  public:

    typedef std::tr1::shared_ptr<VirtualData>       Ptr;
    typedef std::tr1::shared_ptr<const VirtualData> ConstPtr;

    virtual ~VirtualData() {}

    virtual ConstPtr Copy() const { return shared_from_this(); }
    virtual Ptr Copy() { return shared_from_this(); }

    /// Tries to unwrap the data contained in this object using the given type.
    virtual void* Unwrap(const VirtualType& type) const = 0;

    /// Tries to wrap the given data with the given type.
    /** Returns a new VirtualData::Ptr with the data wrapped upon success.
     **
     ** The VirtualData object returned by this method is NOT NECESSARILY the
     ** same as the one that called it. Thus a safe way to use this is like
     ** this:
     **
     ** <code>
     ** VirtualData::Ptr vdata;<br>
     ** // ...<br>
     ** vdata = vdata->Wrap(...);
     ** </code>
     **
     ** Also, a call to this method IS NOT the same as:
     **
     ** <code>
     ** vdata = vdata->wrapper->WrapData(...);
     ** </code>
     **
     ** @param data - a void pointer to the data being wrapped.
     ** @param type - a virtual type got from TypeRegistry<T>::type(), where
     **               T is the original declarated type of the data.
     ** @return A shared pointer to the wrapped data.
     ** @see ugdk::script::LangWrapper
     ** @see ugdk::script::TypeRegistry
     */
    virtual Ptr Wrap(void* data, const VirtualType& type) = 0;

    virtual LangWrapper* wrapper () const = 0;

	/// Tries to execute ourselves as a function in a script language,
    /// passing the given arguments and returning the result.
	virtual Ptr Execute(std::vector<VirtualObj> args) = 0;

	/// Tries to get a attribute with the given name from this object.
	virtual Ptr GetAttribute(const std::string attr_name) const = 0;

  protected:

    VirtualData() {}

};

} /* namespace script */

} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALDATA_H_ */
