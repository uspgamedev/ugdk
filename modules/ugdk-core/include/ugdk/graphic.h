#ifndef UGDK_GRAPHIC_H_
#define UGDK_GRAPHIC_H_

#include <vector>
#include <memory>

namespace ugdk {
namespace graphic {

// Data
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

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_H_
