#ifndef UGDK_GRAPHIC_OPENGL_SHADERPROGRAM_H_
#define UGDK_GRAPHIC_OPENGL_SHADERPROGRAM_H_

#include <string>
#include <list>
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

    class Use {
      public:
        Use(const ShaderProgram* program);
        ~Use();
    
        void SendGeometry(const ugdk::graphic::Geometry&);
        void SendEffect(const ugdk::graphic::VisualEffect&);
        void SendTexture(GLint slot, const Texture* texture);
        void SendVertexBuffer(VertexBuffer* buffer, VertexType type, size_t offset, GLint size = 2);

      private:
        static const ShaderProgram* active_program_;
        const ShaderProgram* program_;
        std::list<GLuint> active_attributes_;
    };

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
};

} // namespace opengl
} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_SHADER_SHADERPROGRAM_H_