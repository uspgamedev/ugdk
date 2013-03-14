#ifndef UGDK_GRAPHIC_OPENGL_SHADERPROGRAM_H_
#define UGDK_GRAPHIC_OPENGL_SHADERPROGRAM_H_

#include <string>
#include "GL/glew.h"
#define NO_SDL_GLEXT

#include <ugdk/graphic.h>

namespace ugdk {
namespace graphic {
namespace opengl {

enum VertexType {
    VERTEX, TEXTURE, COLOR
};

class ShaderProgram {
  public:
    ShaderProgram();
    ~ShaderProgram();

    GLuint id() const { return id_; }
    GLuint matrix_location() const { return matrix_location_; }

    GLuint UniformLocation(const std::string& name) const;
    void SendGeometry(const ugdk::graphic::Geometry&) const;
    void SendTexture(GLint slot, const Texture* texture) const;
    GLuint SendVertexBuffer(VertexBuffer* buffer, VertexType type, size_t offset, GLint size = 2) const;

    bool IsValid() const;
    void AttachShader(const Shader& shader);
    bool SetupProgram();
    void Use() const;

  private:
    static GLuint on_use_;
    GLuint id_;
    GLuint matrix_location_;
    GLuint texture_location_;
};

} // namespace opengl
} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_SHADER_SHADERPROGRAM_H_