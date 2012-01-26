#ifndef VIRTUALOBJ_H_
#define VIRTUALOBJ_H_

namespace ugdk {
namespace script {

class ScriptLangWrapper;

class VirtualObj {
public:
	VirtualObj();
	virtual ~VirtualObj();

	ScriptLangWrapper* wrapper() { return wrapper_; }

protected:
	ScriptLangWrapper* wrapper_;
};

}
}

#endif /* VIRTUALOBJ_H_ */
