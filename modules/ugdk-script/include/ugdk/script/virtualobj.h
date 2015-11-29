
#ifndef UGDK_SCRIPT_VIRTUALOBJ_H_
#define UGDK_SCRIPT_VIRTUALOBJ_H_

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/type.h>
#include <ugdk/script/langwrapper.h>
#include <ugdk/script/virtualprimitive.h>

#include <list>
#include <vector>
#include <map>
#include <string>
#include <type_traits>

#include <cstdio>

namespace ugdk {
namespace script {

class VirtualObj;
class Bind;

/// A proxy class wich represents virtual objects from scripting languages.
/**
 ** Designed for intuitive use.
 **
 ** TODO: explanations and examples.
 */
class VirtualObj {
  public:
    typedef std::list<VirtualObj>               List;
    typedef std::vector<VirtualObj>             Vector;
    typedef std::map<VirtualObj,VirtualObj>     Map;

    /// Builds an <i>empty</i> virtual object.
    /** Attempting to use any method in a virtual object created this way will
     ** result in a segmentation fault.
     */
    explicit VirtualObj() : data_() {}

    explicit VirtualObj(VirtualData::Ptr data) : data_(data) {}

    explicit VirtualObj(LangWrapper* wrapper) : data_(wrapper->NewData()) {}

    ~VirtualObj() {}

    static VirtualObj Create(const char* obj, LangWrapper* wrapper) {
        if (!wrapper) return VirtualObj();
        VirtualObj result(wrapper);
        result.data_->WrapString(obj);
        return result;
    }

    static VirtualObj Create (const std::string& str, LangWrapper* wrapper) {
        return Create(str.c_str(), wrapper);
    }

    /// Acessing

    LangWrapper* wrapper() const { return data_->wrapper(); }
    void* unsafe_data() const { return data_->unsafe_data(); }

    template <class T>
    T value(bool disown = false) const {
        return VirtualPrimitive<T>::value(data_, disown);
    }

    template <class T>
    void set_value(T val, bool disown = false) {
        VirtualPrimitive<T>::set_value(data_, val, disown);
    }

    bool valid() const { return static_cast<bool>(data_); }

    VirtualObj attribute(const VirtualObj& key) const {
        return VirtualObj(data_->GetAttribute(key.data_));
    }

    VirtualObj set_attribute (const VirtualObj& key, const VirtualObj& value) {
        return VirtualObj(
            data_->SetAttribute(key.data_, value.data_)
        );
    }             

    template<typename signature>
    std::function<signature> AsFunction() const {
        return function_helper<signature>::CreateFunction(*this);
    }

    template<typename ...Args>
    VirtualObj Call(Args... args) const {
        VirtualData::Vector arguments;
        arguments.reserve(sizeof...(Args));
        arguments_helper<Args...>::add_to_vector(arguments, this->wrapper(), args...);
        return VirtualObj(data_->Execute(arguments));
    }

    VirtualObj Call(const List& vobj_list) const {
        VirtualData::Vector data_vector;
        data_vector.reserve(vobj_list.size());
        for (const auto& vobj : vobj_list)
            data_vector.push_back(vobj.data_);
        return VirtualObj(data_->Execute(data_vector));
    }

    // Operators

    operator bool() const { return valid(); }

    bool operator<(const VirtualObj& rhs) const {
        return data_.get() < rhs.data_.get();
    }
    template<typename ...Args>
    VirtualObj operator() (Args... args) const {
        return Call<Args...>(args...); 
    }
    VirtualObj operator[] (const VirtualObj& key) const {
        return attribute(key);
    }
    VirtualObj operator[] (const char* key) const {
        return attribute(Create(key, wrapper()));
    }
    VirtualObj operator[] (const std::string& key) const {
        return attribute(Create(key, wrapper()));
    }
    Bind operator|(const std::string& method_name);

  private:
    template<typename ...Args>
    struct arguments_helper;
    
    template<typename T, typename ...Args>
    struct arguments_helper<T, Args...> {
        static bool add_to_vector(VirtualData::Vector& v, LangWrapper* wrapper, T t, Args... args) {
            VirtualObj vobj(wrapper);
            vobj.set_value<T>(t);
            v.emplace_back(vobj.data_);
            return arguments_helper<Args...>::add_to_vector(v, wrapper, args...);
        }
    };

    template<typename ...Args>
    struct arguments_helper<VirtualObj, Args...> {
        static bool add_to_vector(VirtualData::Vector& v, LangWrapper* wrapper, VirtualObj t, Args... args) {
            if(!t || t.wrapper() != wrapper) return false;
            v.emplace_back(t.data_);
            return arguments_helper<Args...>::add_to_vector(v, wrapper, args...);
        }
    };

    template<typename signature, typename Helper = void>
    struct function_helper;

    template<typename R, typename ...Args>
    struct function_helper<R(Args...), typename std::enable_if<is_virtual_primitive<R>::value>::type> {
        static std::function< R(Args...)> CreateFunction(const VirtualObj& data) {
            return [data](Args... args) -> R {
                auto result = data.Call<Args...>(args...);
                return result.value<R>(false);
            };
        }
    };

    template<typename R, typename ...Args>
    struct function_helper<R(Args...), typename std::enable_if<!is_virtual_primitive<R>::value>::type> {
        static std::function< R(Args...)> CreateFunction(const VirtualObj& data) {
            return [data](Args... args) -> R {
                auto result = data.Call<Args...>(args...);
                return result.value<R*>(false);
            };
        }
    };

    template<typename ...Args>
    struct function_helper<void(Args...), void> {
        static std::function<void (Args...)> CreateFunction(const VirtualObj& data) {
            return [data](Args... args) {
                auto callf = std::mem_fn(&VirtualObj::Call<Args...>);
                callf(data, args...);
            };
        }
    };

    VirtualData::Ptr data_;
};

template<>
struct VirtualObj::arguments_helper<> {
    static bool add_to_vector(VirtualData::Vector& v, LangWrapper* wrapper) {
        return true;
    }
};


template <class T, class U>
T ConvertSequence (const U& data_seq) {
    T obj_seq;
    typename U::const_iterator it;
    for (it = data_seq.begin(); it != data_seq.end(); ++it)
        obj_seq.push_back(VirtualObj(*it));
    return obj_seq;
}

template <class T, class U>
T ConvertTable (const U& data_map) {
    T obj_map;
    typename U::const_iterator it;
    for (it = data_map.begin(); it != data_map.end(); ++it) {
        obj_map.insert(std::pair<VirtualObj, VirtualObj>(
            VirtualObj(it->first),
            VirtualObj(it->second)));
    }
    return obj_map;
}

template <>
inline VirtualObj::List VirtualObj::value<VirtualObj::List>(bool /*disown*/) const {
    return ConvertSequence<List>(data_->UnwrapList());
}

template <>
inline VirtualObj::Vector VirtualObj::value<VirtualObj::Vector>(bool /*disown*/) const {
    return ConvertSequence<Vector>(data_->UnwrapVector());
}

template <>
inline VirtualObj::Map VirtualObj::value<VirtualObj::Map>(bool /*disown*/) const {
    return ConvertTable<Map>(data_->UnwrapMap());
}

class Bind {
  public:
    Bind(VirtualObj& obj, const std::string& method_name);

    template<typename ...Args>
    VirtualObj operator() (Args... args) const {
        VirtualObj method = obj_[method_name_];
        auto callf = std::mem_fn(&VirtualObj::Call<VirtualObj, Args...>);
        return callf(method, obj_, args...);
    }

  private:
    Bind& operator=(Bind&); // Bind cannot be copied.

    VirtualObj&   obj_;
    VirtualObj    method_name_;
};

inline Bind VirtualObj::operator|(const std::string& method_name) {
    Bind result(*this, method_name);
    return result;
}

} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
