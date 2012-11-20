#include <ugdk/action/spritetypes.h>

#include <ugdk/util/animationprotocol.h>
#include <ugdk/util/gdd/loader.h>
#include <ugdk/action/spriteanimationframe.h>

namespace ugdk {
namespace action {

action::SpriteAnimationTable* LoadSpriteAnimationTableFromFile(const std::string& filepath) {
    static gdd::Loader<action::SpriteAnimationTable> loader(new AnimationProtocol);
    return loader.Load(filepath);
}

}  // namespace action
}  // namespace ugdk

