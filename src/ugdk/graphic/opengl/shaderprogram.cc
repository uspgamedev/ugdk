#include <ugdk/graphic/opengl/shaderprogram.h>

#include <cstdio>
#include <cassert>
#include "GL/glew.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"

#include <ugdk/graphic/opengl/shader.h>

namespace ugdk {
namespace graphic {
namespace opengl {

ShaderProgram::ShaderProgram() : id_(0) {
    id_ = glCreateProgram();
}
ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id_);
}

unsigned int ShaderProgram::UniformLocation(const std::string& name) const{
    return glGetUniformLocation(id(), name.c_str());
}

bool ShaderProgram::IsValid() const {
    return glIsProgram(id_) == GL_TRUE;
}

void ShaderProgram::AttachShader(const Shader& shader) {
    glAttachShader(id_, shader.id());
}

bool ShaderProgram::SetupProgram() {
    glLinkProgram(id_);

    GLint status;
    glGetProgramiv (id_, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) 	{
        GLint infoLogLength;
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(id_, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    } else {
        matrix_location_ = UniformLocation("geometry_matrix");
    }
    return status == GL_TRUE;
}


} // namespace ugdk
} // namespace graphic
} // namespace opengl
