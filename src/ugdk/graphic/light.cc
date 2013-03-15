#include <GL/glew.h>
#define NO_SDL_GLEXT
#include <ugdk/config/config.h>
#include "SDL_opengl.h"

#include <ugdk/graphic/light.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>

namespace ugdk {
namespace graphic {

Light::Light() : color_() {
    vertexbuffer_ = opengl::VertexBuffer::CreateDefault();
    uvbuffer_ = opengl::VertexBuffer::CreateDefault();
}

void Light::Draw(const Geometry& geometry) {
    // Use our shader
    opengl::ShaderProgram::Use shader_use(VIDEO_MANAGER()->default_shader());

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    shader_use.SendGeometry(geometry * Geometry(-dimension_, 2*dimension_));
    shader_use.SendEffect(VisualEffect(color_));

    // Bind our texture in Texture Unit 0
    shader_use.SendTexture(0, VIDEO_MANAGER()->light_texture());

    // 1rst attribute buffer : vertices
    shader_use.SendVertexBuffer(vertexbuffer_, opengl::VERTEX, 0);

    // 2nd attribute buffer : UVs
    shader_use.SendVertexBuffer(uvbuffer_, opengl::TEXTURE, 0);

    // Draw the triangle !
    glDrawArrays(GL_QUADS, 0, 4); // 12*3 indices starting at 0 -> 12 triangles
}

}  // namespace graphic
}  // namespace ugdk
