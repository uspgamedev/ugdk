#include <ugdk/graphic/shader/shader.h>

#include <cstdio>
#include "GL/glew.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"

namespace ugdk {
namespace graphic {
namespace shader {

Shader::Shader(GLenum shaderType) : id_(0), type_(shaderType) {
    id_ = glCreateShader(shaderType);
}

Shader::~Shader() {
    glDeleteShader(id_);
}

bool Shader::CompileSource(const char* source) {
    glShaderSource(id_, 1, &source, NULL);
    glCompileShader(id_);

    GLint status;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLint infoLogLength;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(id_, infoLogLength, NULL, strInfoLog);

        const char *strShaderType = NULL;
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

} // namespace ugdk
} // namespace graphic
} // namespace shader
