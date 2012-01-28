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
	VirtualObj(LangWrapper* wrapper, VirtualData::Ptr data = NULL) :
	    wrapper_(wrapper),
	    data_(data) {}
	~VirtualObj();

	template <class T>
	T* get() const {
	    return static_cast <T*> (
	        data_->Unwrap(TypeRegistry<T>::FromLang(wrapper_->langID()))
        );
	}

	LangWrapper* wrapper() { return wrapper_; }

	VirtualObj operator() (std::vector<const VirtualObj&> args) {
		VirtualObj ret = VirtualObj(wrapper_);
		ret.data_ = data_->Execute(args);
		return ret;
	}

	VirtualObj operator[] (const std::string attr_name) {
		VirtualObj attr = VirtualObj(wrapper_);
		attr.data_ = data_->GetAttr(attr_name);
		return attr;
	}
	
  private:

	LangWrapper* const wrapper_;
	VirtualData::Ptr data_;
};

}
}

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
