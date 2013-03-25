#ifndef UGDK_GRAPHIC_OPENGL_SHADER_H_
#define UGDK_GRAPHIC_OPENGL_SHADER_H_

#include <string>
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
    bool CompileSource(const std::string& source) {
        return CompileSource(source.c_str());
    }
    bool CompileFile(const std::string& file);

  protected:
    GLuint id_;
    GLuint type_;
};

} // namespace opengl
} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_SHADER_SHADER_H_