
#ifndef UGDK_SCRIPT_VIRTUALOBJ_H_
#define UGDK_SCRIPT_VIRTUALOBJ_H_

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/type.h>
#include <ugdk/script/langwrapper.h>

namespace ugdk {
namespace script {

template <class T>
class VirtualPrimitive { private: VirtualPrimitive() {} };

template <class T>
class VirtualPrimitive<T*> {
  private:
    friend class VirtualObj;
    VirtualPrimitive() {}
    static T* value(const VirtualData::Ptr data) {
        return static_cast <T*> (
            data->Unwrap(TypeRegistry<T>::type())
        );
    }
};

#define DEFINE_SCRIPT_PRIMITIVE_VALUE(type) \
    template <> \
    class VirtualPrimitive<type> { \
      private: \
        friend class VirtualObj; \
        VirtualPrimitive() {} \
        static type value(const VirtualData::Ptr data); \
    }; \
    inline type VirtualPrimitive<type>::value(const VirtualData::Ptr data)

DEFINE_SCRIPT_PRIMITIVE_VALUE(const char*) { return data->UnwrapString(); }
DEFINE_SCRIPT_PRIMITIVE_VALUE(std::string) { return data->UnwrapString(); }
DEFINE_SCRIPT_PRIMITIVE_VALUE(bool) { return data->UnwrapBoolean(); }
DEFINE_SCRIPT_PRIMITIVE_VALUE(int) { return data->UnwrapInteger(); }
DEFINE_SCRIPT_PRIMITIVE_VALUE(long) { return data->UnwrapInteger(); }
DEFINE_SCRIPT_PRIMITIVE_VALUE(short) { return data->UnwrapInteger(); }
DEFINE_SCRIPT_PRIMITIVE_VALUE(float) { return data->UnwrapNumber(); }
DEFINE_SCRIPT_PRIMITIVE_VALUE(double) { return data->UnwrapNumber(); }

/// A proxy class wich represents virtual objects from scripting languages.
/**
 ** Designed for intuitive use.
 **
 ** TODO: explanations and examples.
 */
class VirtualObj {

  public:

    typedef std::pair<VirtualObj,VirtualObj> VirtualEntry;

    /// Builds an <i>empty</i> virtual object.
    /** Attempting to use any method in a virtual object created this way will
     ** result in a segmentation fault.
     */
    explicit VirtualObj() :
        data_() {}

	explicit VirtualObj(VirtualData::Ptr data) :
	    data_(data) {}

	~VirtualObj() {}

	template <class T>
	T value() const {
	    return VirtualPrimitive<T>::value(data_);
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

	operator bool() const { return static_cast<bool>(data_); }

	VirtualObj operator() (std::vector<VirtualObj> args) const;

	VirtualObj attribute(const VirtualObj& key) const {
        return VirtualObj(data_->GetAttribute(key.data_));
	}

    VirtualObj operator[] (const VirtualObj& key) const {
        return attribute(key);
    }
/* TODO
	VirtualObj operator[] (const std::string& key) const {
		return attribute(Create(key, wrapper()));
	}
*/
    VirtualObj operator[] (const char* key) const {
        return attribute(Create(key, wrapper()));
    }
	
	VirtualObj set_attribute (const VirtualObj& key, const VirtualObj& value) {
	    return VirtualObj(
            data_->SetAttribute(key.data_, value.data_)
        );
    }

	VirtualEntry operator,(const VirtualObj& rhs) {
	    return VirtualEntry(*this, rhs);
	}

	VirtualObj operator<<(const VirtualEntry& entry) {
	    return set_attribute(entry.first, entry.second);
	}
/*
    // Does not work anymore.
    template <class T>
    static VirtualObj Create (T obj, LangWrapper* wrapper) {
        if (!wrapper) return VirtualObj();
        VirtualData::Ptr new_data = wrapper->NewData();
        new_data->Wrap(obj); // FIXME
        return VirtualObj(new_data);
    }
*/
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

  private:

	VirtualData::Ptr data_;

};

} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
