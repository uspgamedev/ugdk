#ifndef UGDK_GRAPHIC_SHADER_SHADERPROGRAM_H_
#define UGDK_GRAPHIC_SHADER_SHADERPROGRAM_H_

#include <ugdk/base/types.h>
#include <ugdk/graphic.h>

namespace ugdk {
namespace graphic {
namespace shader {

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    unsigned int id() const { return id_; }

    bool IsValid() const;
    void AttachShader(const Shader& shader);
    bool SetupProgram();

  private:
    unsigned int id_;
};

} // namespace ugdk
} // namespace graphic
} // namespace shader

#endif // UGDK_GRAPHIC_SHADER_SHADERPROGRAM_H_