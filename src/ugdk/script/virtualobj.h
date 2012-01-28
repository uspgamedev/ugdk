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
	VirtualObj(VirtualData::Ptr data = NULL) :
	    data_(data) {}
	~VirtualObj();

	template <class T>
	T* get() const {
	    return static_cast <T*> (
	        data_->Unwrap(TypeRegistry<T>::type())
        );
	}

	LangWrapper* wrapper() { return data_->wrapper(); }

	VirtualObj operator() (std::vector<const VirtualObj&> args) {
		VirtualObj ret = VirtualObj();
		ret.data_ = data_->Execute(args);
		return ret;
	}

	VirtualObj operator[] (const std::string attr_name) {
		VirtualObj attr = VirtualObj();
		attr.data_ = data_->GetAttr(attr_name);
		return attr;
	}
	
  private:

	VirtualData::Ptr data_;
};

}
}

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
