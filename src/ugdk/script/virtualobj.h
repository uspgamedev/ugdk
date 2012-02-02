
#ifndef UGDK_SCRIPT_VIRTUALOBJ_H_
#define UGDK_SCRIPT_VIRTUALOBJ_H_

#include <memory>
#include <vector>
#include <string>

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

    VirtualObj(const VirtualObj& copy) :
        key_(NULL),
        data_(copy.data_) {}

    /// Builds an <i>empty</i> virtual object.
    /** Attempting to use any method in a virtual object created this way will
     ** result in a segmentation fault.
     */
    explicit VirtualObj() :
        key_(NULL),
        data_() {}

	explicit VirtualObj(VirtualData::Ptr data) :
        key_(NULL),
	    data_(data) {}

	~VirtualObj() {}

	VirtualObj& operator =(const VirtualObj& rhs) {
	    data_ = rhs.data_;
	    key_.reset(NULL);
	    return *this;
	}

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
	    if (key_.get() && data_->parent())
	        data_->parent()->SetAttribute(key_->data_, data_);
        return *this;
	}

	LangWrapper* wrapper() const { return data_->wrapper(); }

	VirtualObj operator() (std::vector<VirtualObj> args) const {
		std::vector<VirtualData::Ptr> arglist;
		std::vector<VirtualObj>::iterator it;
		for (it = args.begin(); it != args.end(); ++it) {
			// Wrappers of executed VObj (we) and of the VObjs passed as
		    // arguments must be the same.
			if (wrapper() != it->wrapper())
				return VirtualObj();
			arglist.push_back(it->data_);
		}
		VirtualObj ret(data_->Execute(arglist));
		return ret;
	}

	VirtualObj operator[] (const std::string& attr_name) const {
		VirtualObj attr(data_->GetAttribute(attr_name));
		return attr;
	}
	
	VirtualObj operator[] (const std::string& attr_name) {
        VirtualObj attr(
            data_->GetAttribute(attr_name),
            new VirtualObj(Create(attr_name, wrapper()))
        );
        return attr;
    }

    template <class T>
    static VirtualObj Create (T obj, LangWrapper* wrapper) {
        if (!wrapper) return VirtualObj();
        VirtualData::Ptr new_data = wrapper->NewData();
        new_data->Wrap(obj);
        return VirtualObj(new_data);
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

    static VirtualObj Create (const char* obj, LangWrapper* wrapper) {
        if (!wrapper) return VirtualObj();
        VirtualData::Ptr new_data = wrapper->NewData();
        new_data->Wrap(obj);
        return VirtualObj(new_data);
    }

    static VirtualObj Create (const std::string& str, LangWrapper* wrapper) {
        return Create(str.c_str(), wrapper);
    }

  private:

    explicit VirtualObj(VirtualData::Ptr data, const VirtualObj* key) :
        key_(key),
        data_(data) {}

	std::auto_ptr<const VirtualObj> key_;
	VirtualData::Ptr                data_;

};

}
}

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
