#ifndef UGDK_GRAPHIC_H_
#define UGDK_GRAPHIC_H_

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

// Drawables
class Drawable;

class Label;
class TextBox;
class TexturedRectangle;

// Extra
class Font;
class Light;
class Node;

// Managers
class TextManager;
class Manager;

enum class VertexType;

namespace opengl {
class Shader;
class ShaderProgram;
class VertexBuffer;
}  // namespace opengl

}  // namespace graphic
}  // namespace ugdk

#endif
