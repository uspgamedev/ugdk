#ifndef UGDK_GRAPHIC_OPENGL_SHADERPROGRAM_H_
#define UGDK_GRAPHIC_OPENGL_SHADERPROGRAM_H_

#include <ugdk/internal/opengl.h>
#include <ugdk/graphic.h>
#include <string>

namespace ugdk {
namespace graphic {
namespace opengl {

class ShaderProgram {
  public:
    ShaderProgram();
    ~ShaderProgram();

    GLuint id() const { return id_; }
    GLuint UniformLocation(const std::string& name) const;

    bool IsValid() const;
    void AttachShader(const Shader& shader);
    bool SetupProgram();

  private:
    GLuint id_;
    GLuint matrix_location_;
    GLuint color_location_;
    GLuint texture_location_;

    friend class Canvas;
    friend class ShaderUse;
};

} // namespace opengl
} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_SHADER_SHADERPROGRAM_H_