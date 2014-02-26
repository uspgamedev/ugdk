#include <ugdk/graphic/opengl/shaderuse.h>

#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/opengl/Exception.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>

#include <ugdk/system/config.h>
#include <cstdio>

namespace ugdk {
namespace graphic {
namespace opengl {

const ShaderProgram* ShaderUse::active_program_ = nullptr;

GLuint get_vertextype_location(VertexType type) {
    switch(type) {
    case VERTEX:  return 0;
    case TEXTURE: return 1;
    case COLOR:   return 2;
    default:      return ~0U;
    }
}

ShaderUse::ShaderUse(const ShaderProgram* program) : program_(program), last_attribute_(0) {
    if(active_program_)
        throw love::Exception("There's already a shader program in use.");
    active_program_ = program;
    glUseProgram(program->id_);
}

ShaderUse::~ShaderUse() {
    for(int i = 0; i < last_attribute_; ++i)
        glDisableVertexAttribArray(active_attributes_[i]);
    active_program_ = nullptr;
    glUseProgram(0);
}

void ShaderUse::SendUniform(const std::string& name, float t1) {
    glUniform1f(program_->UniformLocation(name), t1);
}
void ShaderUse::SendUniform(const std::string& name, float t1, float t2) {
    glUniform2f(program_->UniformLocation(name), t1, t2);
}
void ShaderUse::SendUniform(const std::string& name, float t1, float t2, float t3) {
    glUniform3f(program_->UniformLocation(name), t1, t2, t3);
}
void ShaderUse::SendUniform(const std::string& name, float t1, float t2, float t3, float t4) {
    glUniform4f(program_->UniformLocation(name), t1, t2, t3, t4);
}

void ShaderUse::SendGeometry(const glm::mat4& mat) {
    glUniformMatrix4fv(program_->matrix_location_, 1, GL_FALSE, &mat[0][0]);
}

void ShaderUse::SendGeometry(const ugdk::graphic::Geometry& geometry) {
    glUniformMatrix4fv(program_->matrix_location_, 1, GL_FALSE, &geometry.AsMat4()[0][0]);
}
        
void ShaderUse::SendEffect(const ugdk::graphic::VisualEffect& effect) {
    const Color& c = effect.color();
    glUniform4f(program_->color_location_, c.r, c.g, c.b, c.a);
}

void ShaderUse::SendTexture(GLint slot, const Texture* texture) {
    SendTexture(slot, texture->gltexture(), program_->texture_location_);
}
    
void ShaderUse::SendTexture(GLint slot, const Texture* texture, GLuint location) {
    SendTexture(slot, texture->gltexture(), location);
}

void ShaderUse::SendTexture(GLint slot, const Texture* texture, const std::string& location) {
    SendTexture(slot, texture->gltexture(), program_->UniformLocation(location));
}

void ShaderUse::SendTexture(GLint slot, GLuint texture) {
    SendTexture(slot, texture, program_->texture_location_);
}

void ShaderUse::SendTexture(GLint slot, GLuint texture, GLuint location) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(location, slot);
}

void ShaderUse::SendTexture(GLint slot, GLuint texture, const std::string& location) {
    SendTexture(slot, texture, program_->UniformLocation(location));
}

void ShaderUse::SendVertexBuffer(const VertexBuffer* buffer, VertexType type, size_t offset, GLint size, GLsizei stride) {
    VertexBuffer::Bind bind(*buffer);
#ifdef DEBUG
    if(last_attribute_ >= MAX_ATTRIBUTES)
        throw love::Exception("Max attributes used.");
#endif

    GLuint location = get_vertextype_location(type);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(
        location,           // location
        size,               // size
        GL_FLOAT,           // data type
        GL_FALSE,           // normalized?
        stride,             // ammount of bytes to offset for each element. 0 means size*sizeof(type)
        buffer->getPointer(offset) // array buffer offset
    );
    assert(glGetError() == GL_NO_ERROR);
    active_attributes_[last_attribute_++] = location;
}

} // namespace opengl
} // namespace graphic
} // namespace ugdk
