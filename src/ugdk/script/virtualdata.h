
#ifndef UGDK_SCRIPT_VIRTUALDATA_H_
#define UGDK_SCRIPT_VIRTUALDATA_H_

namespace ugdk {

namespace script {

class VirtualType;

/// Abstract class representing virtual script data.
class VirtualData {

  public:

    virtual ~VirtualData() {}

    /// Tries to unwrap the data contained in this object using the given type.
    void* Unwrap(const VirtualType& type) = 0;

    /// Tries to wrap the given data with the given type into this object.
    void Wrap(void* data, const VirtualType& type) = 0;

  protected:

    VirtualData() {}

};

} /* namespace script */

} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALDATA_H_ */
