#include <ugdk/resource/module.h>

#include <ugdk/structure/indexabletable.h>
#include <ugdk/system/compatibility.h>
#include <ugdk/debug/log.h>

#include <ugdk/graphic/textureatlas.h>
#include <ugdk/text/languageword.h>
#include "graphic/gltexture.h"
namespace ugdk {
namespace resource {

static Manager* reference_ = NULL;

bool Initialize(Manager* manager) {
    if(manager) {
        // The manager initialized correctly, so we can use it.
        reference_ = manager;
        return true;
    } else {
        // Error initializing the manager, delete it and don't activate the module.
        delete manager;
        // TODO: log the error.
        return false;
    }
}

void Release() {
    delete reference_;
    reference_ = nullptr;
}

Manager* manager() {
    return reference_;
}

} // namespace resource
} // namespace ugdk
