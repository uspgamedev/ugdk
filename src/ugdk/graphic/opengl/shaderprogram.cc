#include <ugdk/graphic/opengl/shaderprogram.h>

#include <cstdio>

#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/opengl/Exception.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>

namespace ugdk {
namespace graphic {
namespace opengl {

const ShaderProgram* ShaderProgram::Use::active_program_ = NULL;

GLuint get_vertextype_location(VertexType type) {
    switch(type) {
    case VERTEX:  return 0;
    case TEXTURE: return 1;
    case COLOR:   return 2;
    default:      return ~0U;
    }
}

ShaderProgram::Use::Use(const ShaderProgram* program) : program_(program) {
    if(active_program_)
        throw love::Exception("There's already a shader program in use.");
    active_program_ = program;
    glUseProgram(program->id_);
}

ShaderProgram::Use::~Use() {
    for(std::list<GLuint>::const_iterator it = active_attributes_.begin(); it != active_attributes_.end(); ++it) {
        glDisableVertexAttribArray(*it);
    }
    active_program_ = NULL;
    glUseProgram(0);
}

void ShaderProgram::Use::SendGeometry(const ugdk::graphic::Geometry& geometry) {
    float M[16];
    geometry.AsMatrix4x4(M);
    glUniformMatrix4fv(program_->matrix_location_, 1, GL_FALSE, M);
}
        
void ShaderProgram::Use::SendEffect(const ugdk::graphic::VisualEffect& effect) {
    const Color& c = effect.color();
    glUniform4f(program_->color_location_, c.r, c.g, c.b, c.a);
}

void ShaderProgram::Use::SendTexture(GLint slot, const Texture* texture) {
    SendTexture(slot, texture, program_->texture_location_);
}
    
void ShaderProgram::Use::SendTexture(GLint slot, const Texture* texture, GLuint location) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->gltexture());
    glUniform1i(location, slot);
}

void ShaderProgram::Use::SendVertexBuffer(VertexBuffer* buffer, VertexType type, size_t offset, GLint size) {
    VertexBuffer::Bind bind(*buffer);

    GLuint location = get_vertextype_location(type);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(
        location,           // attribute. No particular reason for 0, but must match the layout in the shader.
        size,               // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        buffer->getPointer(offset) // array buffer offset
    );
    this->active_attributes_.push_back(location);
}


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
