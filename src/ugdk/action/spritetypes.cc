#include <ugdk/action/spritetypes.h>

#include <ugdk/util/animationprotocol.h>
#include <ugdk/util/gdd/simpleloader.h>

namespace ugdk {
namespace action {

action::SpriteAnimationTable* LoadSpriteAnimationTableFromFile(const std::string& filepath) {
    static gdd::SimpleLoader<action::SpriteAnimationTable> loader(new AnimationProtocol);
    return loader.Load(filepath);
}

}  // namespace action
}  // namespace ugdk

