#ifndef UGDK_GRAPHIC_OPENGL_SHADER_H_
#define UGDK_GRAPHIC_OPENGL_SHADER_H_

#include "GL/glew.h"
#define NO_SDL_GLEXT

namespace ugdk {
namespace graphic {
namespace opengl {

class Shader {
  public:
    Shader(GLuint shaderType);
    ~Shader();

    GLuint id() const { return id_; }

    bool CompileSource(const char* source);

  protected:
    GLuint id_;
    GLuint type_;
};

} // namespace ugdk
} // namespace graphic
} // namespace opengl

#endif // UGDK_GRAPHIC_SHADER_SHADER_H_