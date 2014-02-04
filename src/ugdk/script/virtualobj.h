
#ifndef UGDK_SCRIPT_VIRTUALOBJ_H_
#define UGDK_SCRIPT_VIRTUALOBJ_H_

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/type.h>
#include <ugdk/script/langwrapper.h>
#include <ugdk/script/virtualprimitive.h>
#include <ugdk/util/uncopyable.h>

#include <list>
#include <vector>
#include <map>
#include <string>

#include <cstdio>

namespace ugdk {
namespace script {

class VirtualObj;
class Bind;
class TempList;

/// A proxy class wich represents virtual objects from scripting languages.
/**
 ** Designed for intuitive use.
 **
 ** TODO: explanations and examples.
 */
class VirtualObj {

  public:

    typedef std::pair<VirtualObj,VirtualObj>    VirtualEntry;
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

    template <class T>
    explicit VirtualObj(LangWrapper* wrapper, T val) : data_(wrapper->NewData()) {
        set_value<T>(val);
    }

    ~VirtualObj() {}

    template <class T>
    T value(bool disown = false) const {
        return VirtualPrimitive<T>::value(data_, disown);
    }

    template <class T>
    void set_value(T val, bool disown = false) {
        VirtualPrimitive<T>::set_value(data_, val, disown);
    }

    template <class T>
    VirtualObj& operator=(T* obj) {
        data_->Wrap(
            static_cast<void*>(obj),
            TypeRegistry<T>::type()
        );
        return *this;
    }

    LangWrapper* wrapper() const { return data_->wrapper(); }

    bool valid() const { return static_cast<bool>(data_); }

    operator bool() const { return valid(); }

    bool operator<(const VirtualObj& rhs) const {
        return data_.get() < rhs.data_.get();
    }

    VirtualObj operator() (const List& args = List()) const;
    VirtualObj operator() (const VirtualObj& arg) const;

    VirtualObj attribute(const VirtualObj& key) const {
        return VirtualObj(data_->GetAttribute(key.data_));
    }

    VirtualObj operator[] (const VirtualObj& key) const {
        return attribute(key);
    }
    VirtualObj operator[] (const char* key) const {
        return attribute(Create(key, wrapper()));
    }
    VirtualObj operator[] (const std::string& key) const {
        return (*this)[key.c_str()];
    }
    
    VirtualObj set_attribute (const VirtualObj& key, const VirtualObj& value) {
        return VirtualObj(
            data_->SetAttribute(key.data_, value.data_)
        );
    }

    TempList operator,(const VirtualObj& rhs) const;

    List& operator,(List& rhs) const {
        rhs.push_front(*this);
        return rhs;
    }

    Bind operator|(const std::string& method_name);

    VirtualObj operator<<(const List& entry) {
        List::const_iterator it = entry.begin();
        return set_attribute(*(it), *(++it));
    }

    template <class T>
    static VirtualObj Create (T* obj, LangWrapper* wrapper) {
        if (!wrapper) return VirtualObj();
        VirtualData::Ptr new_data = wrapper->NewData();
        new_data->Wrap(
            static_cast<void*>(obj),
            TypeRegistry<T>::type()
        );
        return VirtualObj(new_data);
    }

    static VirtualObj Create (const char* obj, LangWrapper* wrapper);

    static VirtualObj Create (const std::string& str, LangWrapper* wrapper) {
        return Create(str.c_str(), wrapper);
    }
    
    void* unsafe_data() const {
        return data_->unsafe_data();
    }

#ifdef UGDK_USING_VARIADIC
    template<typename signature>
    std::function<signature> AsFunction() const {
        return function_helper<signature>::CreateFunction(*this);
    }

    template<typename ...Args>
    VirtualObj Call(Args... args) const {
        VirtualData::Vector arguments;
        createArgumentsVector<Args...>(arguments, wrapper(), args...);
        return VirtualObj(data_->Execute(arguments));
    }
#endif

  private:
#ifdef UGDK_USING_VARIADIC
    
    template<typename ...Args>
    static bool createArgumentsVector(VirtualData::Vector& v, LangWrapper* wrapper) { return true; }

    template<typename T, typename ...Args>
    static bool createArgumentsVector(VirtualData::Vector& v, LangWrapper* wrapper, T t, Args... args) {
        VirtualObj vobj(wrapper);
        vobj.set_value<T>(t);
        return createArgumentsVector<Args...>(v, wrapper, vobj, args...);
    }

    template<typename ...Args>
    static bool createArgumentsVector(VirtualData::Vector& v, LangWrapper* wrapper, const VirtualObj& t, Args... args) {
        if(!t || t.wrapper() != wrapper) return false;
        v.emplace_back(t.data_);
        return createArgumentsVector<Args...>(v, wrapper, args...);
    }

    template<typename signature>
    struct function_helper;

    template<typename R, typename ...Args>
    struct function_helper<R (Args...)> {
        static std::function<R(Args...)> CreateFunction(const VirtualObj& data) {
            return [data](Args... args) -> R {
                auto callf = std::mem_fn(&VirtualObj::Call<Args...>);
                return callf(data, args...).template value<R>(false);
            };
        }
    };

    template<typename ...Args>
    struct function_helper<void (Args...)> {
        static std::function<void (Args...)> CreateFunction(const VirtualObj& data) {
            return [data](Args... args) {
                auto callf = std::mem_fn(&VirtualObj::Call<Args...>);
                callf(data, args...);
            };
        }
    };
#endif

    VirtualData::Ptr data_;

};



template <class T, class U>
T ConvertSequence (const U& data_seq) {
    T obj_seq;
    typename U::const_iterator it;
    for (it = data_seq.begin(); it != data_seq.end(); ++it)
        obj_seq.push_back(VirtualObj(*it));
    return obj_seq;
}

/*static bool VObjLess (const VirtualObj& lhs, const VirtualObj& rhs) {
    return lhs<rhs;
}*/

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

    VirtualObj operator() () const;
    VirtualObj operator() (const VirtualObj& arg) const;
    VirtualObj operator() (std::list<VirtualObj>& args) const;

  private:
    Bind& operator=(Bind&); // Bind cannot be copied.

    VirtualObj&   obj_;
    VirtualObj    method_name_;
};

class TempList {
  public:
    operator VirtualObj::List&() { return l_; }
    TempList& operator,(const VirtualObj& rhs) {
        l_.push_back(rhs);
        return *this;
    }
  private:
    friend class VirtualObj;
    TempList(const VirtualObj& first, const VirtualObj& second) :
        l_() {
        l_.push_back(first);
        l_.push_back(second);
    }
    VirtualObj::List l_;
};

inline TempList VirtualObj::operator,(const VirtualObj& rhs) const {
    return TempList(*this, rhs);
}

inline Bind VirtualObj::operator|(const std::string& method_name) {
    Bind result(*this, method_name);
    return result;
}

} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
