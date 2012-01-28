
#ifndef UGDK_SCRIPT_VIRTUALTYPE_H_
#define UGDK_SCRIPT_VIRTUALTYPE_H_

#include <vector>

#include <ugdk/script/defs.h>

namespace ugdk {

namespace script {

class VirtualType {

  public:

    VirtualType() : types_() {}
    ~VirtualType() {}

    struct swig_type_info* FromLang(LangID id) const {
        return types_[id];
    }

    void RegisterFromLang(struct swig_type_info* info, LangID id) {
        if (!types_[id]) types_[id] = info;
    }

  private:

    typedef std::vector<struct swig_type_info*> TypeFromLang;

    TypeFromLang types_;

};


template <class T>
class TypeRegistry {

  public:

    static TypeRegistry<T>& type () {
        return type_;
    }

  private:

    TypeRegistry () {}

    static VirtualType type_;

};

template <class T>
TypeRegistry<T> TypeRegistry<T>::type_;

} /* namespace script */

} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALTYPE_H_ */
