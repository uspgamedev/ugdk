#include <ugdk/graphic/drawable/functions.h>

#include <GL/glew.h>
#define NO_SDL_GLEXT
#include <glm/glm.hpp>

#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>

namespace ugdk {
namespace graphic {
    
void DrawSquare(const Geometry& geometry, const VisualEffect& effect, const Texture* texture) {
    const glm::mat4& mat = geometry.AsMat4();
    if(mat[3].x > 1 || mat[3].y < -1 || 
        mat[0].x + mat[1].x + mat[3].x < -1 || 
        mat[0].y + mat[1].y + mat[3].y > 1)
        return;
    
    // Use our shader
    opengl::ShaderProgram::Use shader_use(graphic::manager()->shaders().current_shader());

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    shader_use.SendGeometry(mat);
    shader_use.SendEffect(effect);

    // Bind our texture in Texture Unit 0
    shader_use.SendTexture(0, texture);

    // 1rst attribute buffer : vertices
    shader_use.SendVertexBuffer(opengl::VertexBuffer::CreateDefault(), opengl::VERTEX, 0);

    // 2nd attribute buffer : UVs
    shader_use.SendVertexBuffer(opengl::VertexBuffer::CreateDefault(), opengl::TEXTURE, 0);

    // Use the 4 vertices to draw a square.
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

}  // namespace graphic
}  // namespace ugdk
