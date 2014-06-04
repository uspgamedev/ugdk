#include <ugdk/graphic/opengl/shaderprogram.h>

#include <ugdk/debug/log.h>
#include <ugdk/internal/opengl.h>
#include <ugdk/internal/gltexture.h>
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

GLuint ShaderProgram::UniformLocation(const std::string& name) const{
    return glGetUniformLocation(id(), name.c_str());
}

bool ShaderProgram::IsValid() const {
    return glIsProgram(id_) == GL_TRUE;
}

void ShaderProgram::AttachShader(const Shader& shader) {
    shader.Compile();
    glAttachShader(id_, shader.id());
    internal::AssertNoOpenGLError();
}

bool ShaderProgram::SetupProgram() {
    glBindAttribLocation(id_, 0, "vertexPosition");
    glBindAttribLocation(id_, 1, "vertexUV");

    // Clear errors.
    internal::ClearOpenGLErrors();

    glLinkProgram(id_);
    internal::AssertNoOpenGLError();

    GLint status;
    glGetProgramiv (id_, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) 	{
        GLint infoLogLength;
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(id_, infoLogLength, nullptr, strInfoLog);
        debug::Log(debug::LogLevel::WARNING, "UGDK",
                   "Linker failure: ", strInfoLog);
        delete[] strInfoLog;
    } else {
        matrix_location_ = UniformLocation("geometry_matrix");
        texture_location_ = UniformLocation("drawable_texture");
        color_location_ = UniformLocation("effect_color");
    }
    return status == GL_TRUE;
}

} // namespace opengl
} // namespace graphic
} // namespace ugdk
