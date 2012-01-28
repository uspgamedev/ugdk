/// Here are the common definitions of the script system.

#ifndef UGDK_SCRIPT_DEFS_H_
#define UGDK_SCRIPT_DEFS_H_

/// The ID, if defined, of the script language called 'name'.
#define LANG(name) name##Lang

/// Defines an ID for the script language called 'name'.
#define DEFINE_LANG_ID(name,id) \
    const LangID LANG(name) = id;

extern "C" {
/// Used with opaque pointers.
struct swig_type_info;

}

namespace ugdk {

namespace script {

/// Type used for script languages' IDs.
typedef size_t LangID;

/// Script languages ID's definition list.
DEFINE_LANG_ID(Lua,     0);
DEFINE_LANG_ID(Python,  1);

}

}


#endif /* UGDK_SCRIPT_DEFS_H_ */
