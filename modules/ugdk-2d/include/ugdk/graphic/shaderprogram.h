#ifndef UGDK_GRAPHIC_SHADERPROGRAM_H_
#define UGDK_GRAPHIC_SHADERPROGRAM_H_

#include <ugdk/graphic/opengl.h>
#include <ugdk/graphic.h>
#include <string>

namespace ugdk {
namespace graphic {

class ShaderProgram {
  public:
    ShaderProgram();
    ~ShaderProgram();

    GLuint id() const { return id_; }
    GLuint UniformLocation(const std::string& name) const;

    bool IsValid() const;
    void AttachShader(const Shader& shader);
    bool SetupProgram();
    void Validate();

  private:
    void PrintLog(const std::string& title) const;

    GLuint id_;
    GLuint matrix_location_;
    GLuint color_location_;
    GLuint texture_location_;

    friend class Canvas;
    friend class ShaderUse;
};

} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_SHADERPROGRAM_H_