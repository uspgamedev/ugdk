#ifndef UGDK_GRAPHIC_OPENGL_SHADERUSE_H_
#define UGDK_GRAPHIC_OPENGL_SHADERUSE_H_

#include <string>
#include <list>

#include <ugdk/internal/opengl.h>
#include <ugdk/graphic.h>
#include <glm/glm.hpp>

namespace ugdk {
namespace graphic {
namespace opengl {

enum VertexType {
    VERTEX, TEXTURE, COLOR
};

class ShaderUse {
  public:
    ShaderUse(const ShaderProgram* program);
    ~ShaderUse();

    void SendUniform(const std::string& name, float t1);
    void SendUniform(const std::string& name, float t1, float t2);
    void SendUniform(const std::string& name, float t1, float t2, float t3);
    void SendUniform(const std::string& name, float t1, float t2, float t3, float t4);

    void SendGeometry(const glm::mat4&);
    void SendGeometry(const ugdk::graphic::Geometry&);
    void SendEffect(const ugdk::graphic::VisualEffect&);

    void SendTexture(GLint slot, const Texture* texture);
    void SendTexture(GLint slot, const Texture* texture, GLuint location);
    void SendTexture(GLint slot, const Texture* texture, const std::string& location);
    void SendTexture(GLint slot, GLuint texture);
    void SendTexture(GLint slot, GLuint texture, GLuint location);
    void SendTexture(GLint slot, GLuint texture, const std::string& location);

    void SendVertexBuffer(const VertexBuffer* buffer, VertexType type, size_t offset, GLint size = 2, GLsizei stride = 0);

  private:
    static const int MAX_ATTRIBUTES = 4;
    static const ShaderProgram* active_program_;
    const ShaderProgram* program_;
    int last_attribute_;
    GLuint active_attributes_[MAX_ATTRIBUTES];
};

} // namespace opengl
} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_OPENGL_SHADERUSE_H_