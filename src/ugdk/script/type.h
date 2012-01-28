
#ifndef UGDK_SCRIPT_VIRTUALTYPE_H_
#define UGDK_SCRIPT_VIRTUALTYPE_H_

#include <vector>

#include <ugdk/script/defs.h>

namespace ugdk {

namespace script {

class VirtualType {

  public:

    VirtualType() : info_(NULL) {}
    ~VirtualType() {}

    struct swig_type_info* info() const { return info_; }

    void set_info(struct swig_type_info* info) {
        if (!info_) info_ = info;
    }

  private:

    struct swig_type_info* info_;

};

template <class T>
class TypeRegistry {

  public:

    static TypeRegistry<T>& FromLang (LangID id) { return ref_[id]; }

  private:

    TypeRegistry () {}

    static std::vector<VirtualType> ref_;

};

template <class T>
std::vector<VirtualType> TypeRegistry<T>::ref_;

} /* namespace script */

} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALTYPE_H_ */
