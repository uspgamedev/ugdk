
#ifndef UGDK_SCRIPT_VIRTUALPRIMITIVE_H_
#define UGDK_SCRIPT_VIRTUALPRIMITIVE_H_

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/type.h>

namespace ugdk {
namespace script {

template <class T>
class VirtualPrimitive { private: VirtualPrimitive() {} };

template <class T>
class VirtualPrimitive<T*> {
  public:
    static T* value(const VirtualData::Ptr data) {
        return static_cast <T*> (
            data->Unwrap(TypeRegistry<T>::type())
        );
    }
    static void set_value(const VirtualData::Ptr data, T* value) {
        data->Wrap(value, TypeRegistry<T>::type());
    }
  private:
    VirtualPrimitive() {}
};

#define DEFINE_SCRIPT_PRIMITIVE_VALUE(type, name, arg) \
    template <> \
    class VirtualPrimitive<type> { \
      public: \
        static type value(const VirtualData::Ptr data) { \
            return data->Unwrap##name(); \
        } \
        static void set_value(const VirtualData::Ptr data, type value) { \
            data->Wrap##name(arg);  \
        } \
      private: \
        VirtualPrimitive() {} \
    }

#define DEFINE_SCRIPT_SIMPLE_PRIMITIVE_VALUE(type, name) \
    DEFINE_SCRIPT_PRIMITIVE_VALUE(type, name, value)

DEFINE_SCRIPT_PRIMITIVE_VALUE(std::string, String, value.c_str());
DEFINE_SCRIPT_SIMPLE_PRIMITIVE_VALUE(const char*, String);
DEFINE_SCRIPT_SIMPLE_PRIMITIVE_VALUE(bool, Boolean);
DEFINE_SCRIPT_SIMPLE_PRIMITIVE_VALUE(int, Integer);
DEFINE_SCRIPT_SIMPLE_PRIMITIVE_VALUE(long, Integer);
DEFINE_SCRIPT_SIMPLE_PRIMITIVE_VALUE(short, Integer);
DEFINE_SCRIPT_SIMPLE_PRIMITIVE_VALUE(float, Number);
DEFINE_SCRIPT_SIMPLE_PRIMITIVE_VALUE(double, Number);

} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALPRIMITIVE_H_ */
