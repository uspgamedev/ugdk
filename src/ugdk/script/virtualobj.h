
#ifndef UGDK_SCRIPT_VIRTUALOBJ_H_
#define UGDK_SCRIPT_VIRTUALOBJ_H_

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/type.h>
#include <ugdk/script/langwrapper.h>
#include <ugdk/script/virtualprimitive.h>

#include <list>

namespace ugdk {
namespace script {

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

    /// Builds an <i>empty</i> virtual object.
    /** Attempting to use any method in a virtual object created this way will
     ** result in a segmentation fault.
     */
    explicit VirtualObj() :
        data_() {}

	explicit VirtualObj(VirtualData::Ptr data) :
	    data_(data) {}

    explicit VirtualObj(LangWrapper* wrapper) :
        data_(wrapper->NewData()) {}

	~VirtualObj() {}

	template <class T>
	T value() const {
	    return VirtualPrimitive<T>::value(data_);
	}

	template <class T>
	void set_value(T val) {
	    VirtualPrimitive<T>::set_value(data_, val);
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

	VirtualObj operator() (const std::list<VirtualObj>& args) const;

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

  private:

	VirtualData::Ptr data_;

};

class Bind {
  public:
    Bind(VirtualObj& obj, const std::string& method_name) :
        obj_(obj),
        method_name_(obj.wrapper()) {
        method_name_.set_value(method_name);
    }
    VirtualObj operator() (std::list<VirtualObj>& args) const {
        return obj_[method_name_]((obj_, args));
    }
  private:
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
    return Bind(*this, method_name);
}

} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
