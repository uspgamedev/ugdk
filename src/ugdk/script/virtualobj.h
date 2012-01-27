#ifndef UGDK_SCRIPT_VIRTUALOBJ_H_
#define UGDK_SCRIPT_VIRTUALOBJ_H_

namespace ugdk {
namespace script {

class LangWrapper;

class VirtualObj {
public:
	VirtualObj();
	virtual ~VirtualObj();

	LangWrapper* wrapper() { return wrapper_; }

	virtual VirtualObjPtr operator() (/*WAT?!? ARGS!?!*/);
	virtual VirtualObjPtr operator[] () /*TODO LOL*/
	
protected:
	LangWrapper* wrapper_;
};

}
}

#endif /* UGDK_SCRIPT_VIRTUALOBJ_H_ */
