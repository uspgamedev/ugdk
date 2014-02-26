#include <ugdk/graphic/opengl/shaderprogram.h>

#include <cstdio>

#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/opengl/Exception.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>

#include <ugdk/system/config.h>

namespace ugdk {
namespace graphic {
namespace opengl {

ShaderProgram::ShaderProgram() : id_(0) {
    id_ = glCreateProgram();
}
ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id_);
}

GLuint ShaderProgram::UniformLocation(const std::string& name) const{
    return glGetUniformLocation(id(), name.c_str());
}

bool ShaderProgram::IsValid() const {
    return glIsProgram(id_) == GL_TRUE;
}

void ShaderProgram::AttachShader(const Shader& shader) {
    shader.Compile();
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
        glGetProgramInfoLog(id_, infoLogLength, nullptr, strInfoLog);
        printf("Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    } else {
        matrix_location_ = UniformLocation("geometry_matrix");
        texture_location_ = UniformLocation("drawable_texture");
        color_location_ = UniformLocation("effect_color");
    }
    glBindAttribLocation(id_, 0, "vertexPosition");
    glBindAttribLocation(id_, 1, "vertexUV");
    return status == GL_TRUE;
}

} // namespace opengl
} // namespace graphic
} // namespace ugdk
