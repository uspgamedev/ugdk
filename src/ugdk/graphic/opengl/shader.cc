#include <ugdk/graphic/opengl/shader.h>

#include <cstdio>
#include <fstream>
#include <sstream>

#include <ugdk/system/config.h>

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
#ifdef ANDROID
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
#ifndef ANDROID
        case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
#endif
        case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        default: strShaderType = "unknown"; break;
        }

        printf("Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        printf("============ SOURCE: \n%s\n", source_.c_str()); 
        delete[] strInfoLog;
    }
    return status == GL_TRUE;
}

} // namespace opengl
} // namespace graphic
} // namespace ugdk
