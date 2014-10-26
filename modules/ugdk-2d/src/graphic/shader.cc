#include <ugdk/graphic/shader.h>

#include <ugdk/debug/log.h>

#include <fstream>
#include <sstream>

namespace ugdk {
namespace graphic {

Shader::Shader(GLenum shaderType) : id_(0), type_(shaderType) {
    id_ = glCreateShader(shaderType);
}

Shader::~Shader() {
    glDeleteShader(id_);
}

void Shader::GenerateSource() {
#ifdef UGDK_USING_GLES
    source_ = "#version 100" "\n";
#else
    source_ = "#version 120" "\n";
    source_ += "#define highp " "\n";
    source_ += "#define mediump " "\n";
    source_ += "#define lowp " "\n";
#endif
    if(type_ == GL_VERTEX_SHADER) {
        source_ += "#define in attribute" "\n";
        source_ += "#define out varying" "\n";
    } else {
        source_ += "#define in varying" "\n";
    }
    
    source_ += "uniform highp mat4 geometry_matrix;" "\n";

    if(type_ == GL_VERTEX_SHADER) {
        source_ += "in highp vec2 vertexPosition;" "\n";
        source_ += "in highp vec2 vertexUV;" "\n";
    }

    source_ += blocks_;
    source_ += "void main() {" "\n";
    source_ += main_;
    source_ += "}";
}

bool Shader::Compile() const {

    internal::ClearOpenGLErrors();

    const char* source = source_.c_str();
    glShaderSource(id_, 1, &source, nullptr);
    internal::AssertNoOpenGLError();
    glCompileShader(id_);
    internal::AssertNoOpenGLError();

    GLint status;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLint infoLogLength;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(id_, infoLogLength, nullptr, strInfoLog);

        const char *strShaderType = nullptr;
        switch(type_) {
        case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
        case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        default: strShaderType = "unknown"; break;
        }

        debug::Log(debug::WARNING, "UGDK",
                   "Compiler failure in ", strShaderType, " shader: ", strInfoLog);
        debug::Log(debug::WARNING, "UGDK",
                   "Source: ", source_);
        delete[] strInfoLog;
    }
    return status == GL_TRUE;
}

} // namespace graphic
} // namespace ugdk
