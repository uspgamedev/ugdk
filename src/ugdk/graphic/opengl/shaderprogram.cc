#include <ugdk/graphic/opengl/shaderprogram.h>

#include <cstdio>

#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>

namespace ugdk {
namespace graphic {
namespace opengl {

GLuint ShaderProgram::on_use_ = 0;

ShaderProgram::BufferDataLocation::BufferDataLocation(VertexType type) {
    switch(type) {
    case VERTEX: 
        location_ = 0; break;
    case TEXTURE:
        location_ = 1; break;
    case COLOR:
        location_ = 2; break;
    default:
        location_ = ~0U;
    }
    if(location_ != ~0U) {
        glEnableVertexAttribArray(location_);
    }
}

ShaderProgram::BufferDataLocation::BufferDataLocation(const BufferDataLocation& other) {
    location_ = other.location_;
    const_cast<BufferDataLocation&>(other).location_ = ~0U;
}

ShaderProgram::BufferDataLocation::~BufferDataLocation() {
    if(location_ != ~0U) {
        glDisableVertexAttribArray(location_);
    }
    location_ = ~0U;
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

void ShaderProgram::SendGeometry(const ugdk::graphic::Geometry& geometry) const {
    float M[16];
    geometry.AsMatrix4x4(M);
    glUniformMatrix4fv(matrix_location_, 1, GL_FALSE, M);
}
    
void ShaderProgram::SendTexture(GLint slot, const Texture* texture) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D + slot, texture->gltexture());
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(texture_location_, slot);
}

ShaderProgram::BufferDataLocation ShaderProgram::SendVertexBuffer(VertexBuffer* buffer, VertexType type, size_t offset, GLint size) const {
    VertexBuffer::Bind bind(*buffer);

    BufferDataLocation location(type);
    glVertexAttribPointer(
        location.location_, // attribute. No particular reason for 0, but must match the layout in the shader.
        size,               // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        buffer->getPointer(offset) // array buffer offset
    );
    return location;
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
    }
    return status == GL_TRUE;
}

void ShaderProgram::Use() const {
    if(on_use_ == id_) return;
    on_use_ = id_;
    glUseProgram(id_);
}

} // namespace opengl
} // namespace graphic
} // namespace ugdk
