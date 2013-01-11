#ifndef UGDK_GRAPHIC_SHADER_SHADER_H_
#define UGDK_GRAPHIC_SHADER_SHADER_H_

#include <ugdk/base/types.h>

namespace ugdk {
namespace graphic {
namespace shader {

class Shader {
  public:
    Shader(unsigned int shaderType);
    ~Shader();

    unsigned int id() const { return id_; }

    bool CompileSource(const char* source);

  protected:
    unsigned int id_;
    unsigned int type_;
};

} // namespace ugdk
} // namespace graphic
} // namespace shader

#endif // UGDK_GRAPHIC_SHADER_SHADER_H_