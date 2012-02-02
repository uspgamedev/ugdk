
#ifndef UGDK_SCRIPT_VIRTUALOBJ_H_
#define UGDK_SCRIPT_VIRTUALOBJ_H_

#ifdef ECLIPSE_BOOST_WORKAROUND_INCLUDE
#include <boost/tr1/memory.hpp>
#else
#include <memory>
#endif

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

	VirtualObj operator[] (const std::string& key) const {
		VirtualObj attr(data_->GetAttribute(key));
		return attr;
	}
	
	VirtualObj set_attribute (const std::string& key, const VirtualObj& value) {
	    return data_->SetAttribute(Create(key, wrapper()).data_, value.data_);
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

	VirtualData::Ptr                data_;

};

}
}

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
