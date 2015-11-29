#ifndef UGDK_GRAPHIC_H_
#define UGDK_GRAPHIC_H_

#include <ugdk/action.h>
#include <ugdk/structure.h>
#include <vector>

namespace ugdk {
namespace graphic {

// Data
class Geometry;
class VisualEffect;
class VertexData;
class Primitive;
class PrimitiveController;
class PrimitiveControllerPosition;
class TextureAtlas;
struct PixelSurface;
class GLTexture;
class Shader;
class ShaderProgram;

class Canvas;
class RenderTarget;
class RenderTexture;
class TextureUnit;

// Extra
class Light;

// Managers
class Manager;

enum class VertexType;

class SpriteAnimationFrame;
using SpriteAnimation = ::std::vector<SpriteAnimationFrame*>;
using SpriteAnimationPlayer = ::ugdk::action::AnimationPlayer<SpriteAnimationFrame>;
using SpriteAnimationTable = ::ugdk::structure::IndexableTable<SpriteAnimation*>;

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_H_
