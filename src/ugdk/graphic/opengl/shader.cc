#include <ugdk/graphic/opengl/shader.h>

#include <cstdio>
#include <fstream>
#include <sstream>

namespace ugdk {
namespace graphic {
namespace opengl {

Shader::Shader(GLenum shaderType) : id_(0), type_(shaderType) {
    id_ = glCreateShader(shaderType);
}

Shader::~Shader() {
    glDeleteShader(id_);
}

void Shader::GenerateSource() {
    source_ = "#version 120" "\n";
    if(type_ == GL_VERTEX_SHADER) {
        source_ += "#define in attribute" "\n";
        source_ += "#define out varying" "\n";
    } else {
        source_ += "#define in varying" "\n";
    }
    
    source_ += "uniform mat4 geometry_matrix;" "\n";

    if(type_ == GL_VERTEX_SHADER) {
        source_ += "in vec2 vertexPosition;" "\n";
        source_ += "in vec2 vertexUV;" "\n";
    }

    source_ += blocks_;
    source_ += "void main() {" "\n";
    source_ += main_;
    source_ += "}";
}

bool Shader::Compile() const {
    const char* source = source_.c_str();
    glShaderSource(id_, 1, &source, nullptr);
    glCompileShader(id_);

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
        case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
        case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        }

        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        delete[] strInfoLog;
    }
    return status == GL_TRUE;
}

} // namespace opengl
} // namespace graphic
} // namespace ugdk
