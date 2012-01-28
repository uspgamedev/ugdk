#ifndef UGDK_SCRIPT_VIRTUALOBJ_H_
#define UGDK_SCRIPT_VIRTUALOBJ_H_

#include <memory>

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/type.h>

namespace ugdk {
namespace script {

class LangWrapper;

class VirtualObj {
  public:
	VirtualObj();
	virtual ~VirtualObj();

	template <class T>
	T* get() {
	    return data_->Unwrap(TypeRegistry<T>::FromLang(wrapper_->langID()));
	}

	LangWrapper* wrapper() { return wrapper_; }

	virtual VirtualObjPtr operator() (/*WAT?!? ARGS!?!*/);
	virtual VirtualObjPtr operator[] (); /*TODO LOL*/
	
  private:
	LangWrapper* wrapper_;
	std::tr1::shared_ptr<VirtualData> data_;
};

}
}

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
