
#ifndef UGDK_SCRIPT_VIRTUALDATA_H_
#define UGDK_SCRIPT_VIRTUALDATA_H_

#include <ugdk/portable/tr1.h>

#include FROM_TR1(memory)

#include <vector>

#include <ugdk/base/types.h>
#include <ugdk/script/type.h>

namespace ugdk {

namespace script {

class VirtualObj;
class LangWrapper;

/// Abstract class representing virtual script data.
class VirtualData : public std::tr1::enable_shared_from_this<VirtualData> {

  public:

    typedef std::tr1::shared_ptr<VirtualData>       Ptr;
    typedef std::tr1::shared_ptr<const VirtualData> ConstPtr;
    typedef std::tr1::weak_ptr<VirtualData>         WeakPtr;

    virtual ~VirtualData() {}

    Ptr Copy() {
        return shared_from_this();
    }

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
     ** @param data - a void pointer to the data being wrapped.
     ** @param type - a virtual type got from TypeRegistry<T>::type(), where
     **               T is the original declarated type of the data.
     ** @return A shared pointer to the wrapped data.
     ** @see ugdk::script::TypeRegistry
     ** @depracated
     */
    virtual void Wrap(void* data, const VirtualType& type) = 0;
    virtual void Wrap(const char* str) = 0;
    virtual void Wrap(bool boolean) = 0;
    virtual void Wrap(int number) = 0;
    virtual void Wrap(double number) = 0;

    virtual LangWrapper* wrapper () const = 0;

	/// Tries to execute ourselves as a function in a script language,
    /// passing the given arguments and returning the result.
	virtual Ptr Execute(const std::vector<Ptr>& args) = 0;

	/// Tries to get a attribute with the given name from this object.
	/**
	 */
	virtual Ptr GetAttribute(Ptr key) = 0;

	virtual Ptr SetAttribute(Ptr key, Ptr value) = 0;

    virtual void AddToBuffer() {}

  protected:

    VirtualData() {}

  private:



};

} /* namespace script */

} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALDATA_H_ */
