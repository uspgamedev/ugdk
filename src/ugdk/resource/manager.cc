#include <ugdk/resource/manager.h>

#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/resource/genericcontainer.h>
#include <ugdk/resource/module.h>
#include <ugdk/system/engine.h>
#include <ugdk/util/animationprotocol.h>
#include <ugdk/util/languageword.h>

#ifdef DEBUG
#include <cstdio>
#include <typeinfo>
#define TOSTRING(X) typeid(X).name()
#endif



bool type_info_cmp(const std::type_info* a, const std::type_info* b) {
    return (a->before(*b) != 0);
}

namespace ugdk {
namespace resource {

Manager::Manager() : containers_(type_info_cmp) {}

Manager::~Manager() {
    for(ResourceMap::iterator it = containers_.begin();
        it != containers_.end(); ++it)
        delete it->second;
}

} // namespace resource
} // namespace ugdk
