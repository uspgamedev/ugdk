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
	VirtualObj(LangWrapper* wrapper) :
	    wrapper_(wrapper),
	    data_(NULL) {}
	virtual ~VirtualObj();

	template <class T>
	T* get() {
	    return static_cast <T*> (
	        data_->Unwrap(TypeRegistry<T>::FromLang(wrapper_->langID()))
        );
	}

	LangWrapper* wrapper() { return wrapper_; }

	virtual VirtualObj operator() (std::vector<const VirtualObj&> args);
	virtual VirtualObj& operator[] (const std::string attr_name);
	
  private:

	LangWrapper* const wrapper_;
	VirtualData::Ptr data_;
};

}
}

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
