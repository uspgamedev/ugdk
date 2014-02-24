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
struct is_virtual_primitive { static const bool value = false; };

template <typename T>
class VirtualPrimitive { 
  public:
    static_assert(is_virtual_primitive<T>::value, "Unsupported type.");
};

template <typename T>
class VirtualPrimitive<T*> {
  public:
    static T* value(const VirtualData::Ptr data, bool disown) {
        return static_cast <T*> (
            data->Unwrap(TypeRegistry<T>::type(), disown)
        );
    }
    static void set_value(const VirtualData::Ptr data, const T* value, bool disown) {
        data->Wrap(const_cast<void*>(static_cast<const void*>(value)), TypeRegistry<T>::type(), disown);
    }
  private:
    VirtualPrimitive() {}
};

template <typename T>
struct is_virtual_primitive<T*> { static const bool value = true; };

template <typename T>
class VirtualPrimitive<T&> {
public:
    static T& value(const VirtualData::Ptr data, bool disown) {
        return *static_cast<T*>(
            data->Unwrap(TypeRegistry<T>::type(), disown)
        );
    }
    static void set_value(const VirtualData::Ptr data, const T& value, bool disown) {
        data->Wrap(const_cast<void*>(static_cast<const void*>(&value)), TypeRegistry<T>::type(), disown);
    }
private:
    VirtualPrimitive() {}
};

template <typename T>
struct is_virtual_primitive<T&> { static const bool value = true; };

template <typename T, typename S>
inline T CheckAndCast (S value) {
    return static_cast<T>(value);
}

template <>
inline std::string CheckAndCast<std::string, const char*> (const char* value) {
    return value ? value : "";
}

#define DEFINE_SCRIPT_PRIMITIVE_VALUE(type, name, arg) \
    template <> \
    class VirtualPrimitive<type> { \
      public: \
        static type value(const VirtualData::Ptr data, bool) { \
            return CheckAndCast<type>(data->Unwrap##name()); \
        } \
        static void set_value(const VirtualData::Ptr data, type value, bool) { \
            data->Wrap##name(arg);  \
        } \
      private: \
        VirtualPrimitive() {} \
    }; \
    template <> \
    struct is_virtual_primitive<type> { static const bool value = true; }

#define DEFINE_SCRIPT_SIMPLE_PRIMITIVE_VALUE(type, name) \
    DEFINE_SCRIPT_PRIMITIVE_VALUE(type, name, value)

DEFINE_SCRIPT_PRIMITIVE_VALUE(std::string, String, value.c_str());
// Because I can!
TYPENAME_MAP(DEFINE_SCRIPT_SIMPLE_PRIMITIVE_VALUE)

#undef TYPENAME_MAP

} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALPRIMITIVE_H_ */
