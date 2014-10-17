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
typedef ::ugdk::action::AnimationPlayer<SpriteAnimationFrame> SpriteAnimationPlayer;
typedef ::ugdk::structure::IndexableTable<std::vector<SpriteAnimationFrame*>*> SpriteAnimationTable;

namespace opengl {
class Shader;
class ShaderProgram;
class VertexBuffer;
}  // namespace opengl

}  // namespace graphic
}  // namespace ugdk

#endif
