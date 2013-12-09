#ifndef UGDK_GRAPHIC_OPENGL_SHADER_H_
#define UGDK_GRAPHIC_OPENGL_SHADER_H_

#include <string>
#include <ugdk/internal/opengl.h>

namespace ugdk {
namespace graphic {
namespace opengl {

class Shader {
  public:
    Shader(GLuint shaderType);
    ~Shader();

    GLuint id() const { return id_; }

    void AddLineInMain(const std::string& line) { main_ += line; }
    void AddCodeBlock(const std::string& block) { blocks_ += block; }

    void GenerateSource();

    bool Compile() const;

    /// Sets the source code for this shader.
    /** Warning: Overrides any generated source. 
        You MUST use at least the variable names UGDK expects by default in SetupProgram.
        You know what you doing. For great justice! */
    void set_source(const std::string& source) { source_ = source; }

  protected:
    GLuint id_;
    GLuint type_;
    std::string source_;

    std::string blocks_;
    std::string main_;
};

} // namespace opengl
} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_SHADER_SHADER_H_