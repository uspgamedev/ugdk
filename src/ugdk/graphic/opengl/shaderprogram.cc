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

const ShaderProgram* ShaderProgram::Use::active_program_ = nullptr;

GLuint get_vertextype_location(VertexType type) {
    switch(type) {
    case VERTEX:  return 0;
    case TEXTURE: return 1;
    case COLOR:   return 2;
    default:      return ~0U;
    }
}

ShaderProgram::Use::Use(const ShaderProgram* program) : program_(program), last_attribute_(0) {
    if(active_program_)
        throw love::Exception("There's already a shader program in use.");
    active_program_ = program;
    glUseProgram(program->id_);
}

ShaderProgram::Use::~Use() {
    for(int i = 0; i < last_attribute_; ++i)
        glDisableVertexAttribArray(active_attributes_[i]);
    active_program_ = nullptr;
    glUseProgram(0);
}

void ShaderProgram::Use::SendGeometry(const glm::mat4& mat) {
    glUniformMatrix4fv(program_->matrix_location_, 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::Use::SendGeometry(const ugdk::graphic::Geometry& geometry) {
    glUniformMatrix4fv(program_->matrix_location_, 1, GL_FALSE, &geometry.AsMat4()[0][0]);
}
        
void ShaderProgram::Use::SendEffect(const ugdk::graphic::VisualEffect& effect) {
    const Color& c = effect.color();
    glUniform4f(program_->color_location_, c.r, c.g, c.b, c.a);
}

void ShaderProgram::Use::SendTexture(GLint slot, const Texture* texture) {
    SendTexture(slot, texture->gltexture(), program_->texture_location_);
}
    
void ShaderProgram::Use::SendTexture(GLint slot, const Texture* texture, GLuint location) {
    SendTexture(slot, texture->gltexture(), location);
}

void ShaderProgram::Use::SendTexture(GLint slot, const Texture* texture, const std::string& location) {
    SendTexture(slot, texture->gltexture(), program_->UniformLocation(location));
}

void ShaderProgram::Use::SendTexture(GLint slot, GLuint texture) {
    SendTexture(slot, texture, program_->texture_location_);
}

void ShaderProgram::Use::SendTexture(GLint slot, GLuint texture, GLuint location) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(location, slot);
}

void ShaderProgram::Use::SendTexture(GLint slot, GLuint texture, const std::string& location) {
    SendTexture(slot, texture, program_->UniformLocation(location));
}

void ShaderProgram::Use::SendVertexBuffer(const VertexBuffer* buffer, VertexType type, size_t offset, GLint size) {
    VertexBuffer::Bind bind(*buffer);
#ifdef DEBUG
    if(last_attribute_ >= MAX_ATTRIBUTES)
        throw love::Exception("Max attributes used.");
#endif

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
    active_attributes_[last_attribute_++] = location;
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
