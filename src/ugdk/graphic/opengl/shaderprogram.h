#ifndef UGDK_GRAPHIC_OPENGL_SHADERPROGRAM_H_
#define UGDK_GRAPHIC_OPENGL_SHADERPROGRAM_H_

#include <string>
#include <ugdk/base/types.h>
#include <ugdk/graphic.h>

namespace ugdk {
namespace graphic {
namespace opengl {

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    unsigned int id() const { return id_; }
    unsigned int matrix_location() const { return matrix_location_; }

    unsigned int UniformLocation(const std::string& name) const;
    bool IsValid() const;
    void AttachShader(const Shader& shader);
    bool SetupProgram();

  private:
    unsigned int id_;
    unsigned int matrix_location_;
};

} // namespace ugdk
} // namespace graphic
} // namespace opengl

#endif // UGDK_GRAPHIC_SHADER_SHADERPROGRAM_H_