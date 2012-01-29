#ifndef UGDK_SCRIPT_VIRTUALOBJ_H_
#define UGDK_SCRIPT_VIRTUALOBJ_H_

#include <memory>
#include <vector>
#include <string>
#include <ugdk/script/virtualdata.h>
#include <ugdk/script/type.h>

namespace ugdk {
namespace script {

class LangWrapper;

class VirtualObj {

  public:

    VirtualObj() :
        data_() {}

	explicit VirtualObj(VirtualData::Ptr data) :
	    data_(data) {}

	~VirtualObj();

	template <class T>
	T* get() const {
	    return static_cast <T*> (
	        data_->Unwrap(TypeRegistry<T>::type())
        );
	}

	template <class T>
	void set(T* obj) {
	    data_ = data_->Wrap(
            static_cast<void*>(obj),
            TypeRegistry<T>::type()
        );
	}

	LangWrapper* wrapper() const { return data_->wrapper(); }

	VirtualObj operator() (std::vector<VirtualObj> args) const {
		VirtualObj ret(data_->Execute(args));
		return ret;
	}

	VirtualObj operator[] (const std::string attr_name) const {
		VirtualObj attr(data_->GetAttr(attr_name));
		return attr;
	}
	
  private:

	VirtualData::Ptr data_;

};

}
}

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
