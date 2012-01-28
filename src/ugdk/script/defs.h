
#ifndef UGDK_SCRIPT_DEFS_H_
#define UGDK_SCRIPT_DEFS_H_

#define LANG(name) name##Lang

#define DEFINE_LANG_ID(name,id) \
    const LangID LANG(name) = id;

extern "C" {

/// Used with opaque pointers.
struct swig_type_info;

}

#include <memory>

namespace ugdk {

namespace script {

typedef size_t LangID;

DEFINE_LANG_ID(Lua,     0);
DEFINE_LANG_ID(Python,  1);

class VirtualObj;

typedef std::tr1::shared_ptr<VirtualObj> VirtualObjPtr;

}

}


#endif /* UGDK_SCRIPT_DEFS_H_ */
