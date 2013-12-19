
#ifndef UGDK_SCRIPT_VIRTUALPRIMITIVE_H_
#define UGDK_SCRIPT_VIRTUALPRIMITIVE_H_

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/type.h>

namespace ugdk {
namespace script {

#define TYPENAME_MAP(definition) \
    definition(const char*, String); \
    definition(bool, Boolean); \
    definition(int, Integer); \
    definition(long, Integer); \
    definition(short, Integer); \
    definition(float, Number); \
    definition(double, Number);

template <typename T>
class VirtualPrimitive { private: VirtualPrimitive() {} };

template <typename T>
class VirtualPrimitive<T*> {
  public:
    static T* value(const VirtualData::Ptr data, bool disown) {
        return static_cast <T*> (
            data->Unwrap(TypeRegistry<T>::type(), disown)
        );
    }
    static void set_value(const VirtualData::Ptr data, T* value) {
        data->Wrap(value, TypeRegistry<T>::type());
    }
  private:
    VirtualPrimitive() {}
};

template <typename T, typename S>
T CheckAndCast (S value) {
    return static_cast<T>(value);
}

template <>
std::string CheckAndCast<std::string, const char*> (const char* value) {
    return value ? value : "";
}

#define DEFINE_SCRIPT_PRIMITIVE_VALUE(type, name, arg) \
    template <> \
    class VirtualPrimitive<type> { \
      public: \
        static type value(const VirtualData::Ptr data, bool) { \
            return CheckAndCast<type>(data->Unwrap##name()); \
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
// Because I can!
TYPENAME_MAP(DEFINE_SCRIPT_SIMPLE_PRIMITIVE_VALUE)

#undef TYPENAME_MAP

} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALPRIMITIVE_H_ */
