#include <GL/glew.h>
#define NO_SDL_GLEXT
#include <ugdk/config/config.h>
#include "SDL_opengl.h"

#include <ugdk/graphic/light.h>

#include <ugdk/system/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>

namespace ugdk {
namespace graphic {

Light::Light() : dimension_(1.0, 1.0) {
    static const GLfloat buffer_data[] = { 
        -1.0f,-1.0f, 
         1.0f,-1.0f, 
         1.0f, 1.0f, 
        -1.0f, 1.0f
    };
    vertexbuffer_ = opengl::VertexBuffer::Create(sizeof(buffer_data), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    {
        opengl::VertexBuffer::Bind bind(*vertexbuffer_);
        opengl::VertexBuffer::Mapper mapper(*vertexbuffer_);

        GLfloat *indices = static_cast<GLfloat*>(mapper.get());
        if (indices)
            memcpy(indices, buffer_data, sizeof(buffer_data));
    }
}

Light::~Light() {
    delete vertexbuffer_;
}

void Light::Draw(const Geometry& geometry) {
    // Use our shader
    opengl::ShaderProgram* shader = VIDEO_MANAGER()->light_shader();
    opengl::ShaderProgram::Use shader_use(shader);

    shader_use.SendGeometry(geometry * Geometry(math::Vector2D(), dimension_));
    shader_use.SendEffect(VisualEffect(color_));

    // 1rst attribute buffer : vertices
    shader_use.SendVertexBuffer(vertexbuffer_, opengl::VERTEX, 0);

    // Draw the triangle !
    glDrawArrays(GL_QUADS, 0, 4); // 12*3 indices starting at 0 -> 12 triangles
}

}  // namespace graphic
}  // namespace ugdk
