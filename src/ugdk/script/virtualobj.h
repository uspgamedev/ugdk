
#ifndef UGDK_SCRIPT_VIRTUALOBJ_H_
#define UGDK_SCRIPT_VIRTUALOBJ_H_

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/type.h>
#include <ugdk/script/langwrapper.h>

namespace ugdk {
namespace script {

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
	T* value() const {
	    return static_cast <T*> (
	        data_->Unwrap(TypeRegistry<T>::type())
        );
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

	VirtualObj operator() (std::vector<VirtualObj> args) const;

	VirtualObj attribute(const VirtualObj& key) const {
        return VirtualObj(data_->GetAttribute(key.data_));
	}

    VirtualObj operator[] (const VirtualObj& key) const {
        return attribute(key);
    }

	VirtualObj operator[] (const std::string& key) const {
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

}
}

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
