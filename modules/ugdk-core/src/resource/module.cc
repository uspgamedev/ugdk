#include <ugdk/resource/module.h>

#include <ugdk/structure/indexabletable.h>
#include <ugdk/system/compatibility.h>
#include <ugdk/debug/log.h>

#include <ugdk/graphic/textureatlas.h>
#include <ugdk/text/languageword.h>
#include "graphic/gltexture.h"
namespace ugdk {
namespace resource {

static std::unique_ptr<Manager> reference_;

bool is_active() {
    return static_cast<bool>(reference_);
}

bool Initialize(std::unique_ptr<Manager> manager) {
    if(manager) {
        // The manager initialized correctly, so we can use it.
        reference_ = std::move(manager);
;
        return true;
    } else {
        // Error initializing the manager, delete it and don't activate the module.
        manager.reset();
        // TODO: log the error.
        return false;
    }
}

void Release() {
     reference_.reset();
}

resource::Manager& manager() {
    return *(reference_.get());
}

} // namespace resource
} // namespace ugdk
