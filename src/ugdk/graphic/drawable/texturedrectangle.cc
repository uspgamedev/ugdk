#include <ugdk/config/config.h>
#include "GL/glew.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"
#include <cassert>

#include <ugdk/graphic/drawable/texturedrectangle.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/opengl/shaderprogram.h>

namespace ugdk {
namespace graphic {

TexturedRectangle::TexturedRectangle(Texture* texture) 
    :   size_(static_cast<double>(texture->width()), static_cast<double>(texture->height())), 
        texture_(texture) {

    glGenVertexArrays(1, &vertex_array_);
    glGenBuffers(1, &position_buffer_);
    glGenBuffers(1, &texture_buffer_);
}

TexturedRectangle::TexturedRectangle(Texture* texture, const ugdk::math::Vector2D& size) 
    : size_(size), texture_(texture) {
    glGenVertexArrays(1, &vertex_array_);
    glGenBuffers(1, &position_buffer_);
    glGenBuffers(1, &texture_buffer_);
}

TexturedRectangle::~TexturedRectangle() {
    glDeleteVertexArrays(1, &vertex_array_);
    glDeleteBuffers(1, &position_buffer_);
    glDeleteBuffers(1, &texture_buffer_);
}

void TexturedRectangle::Update(double dt) {}

void TexturedRectangle::Draw(const Geometry& modifier, const VisualEffect& effect) const {
    ugdk::math::Vector2D origin, target(size_);

    origin -= hotspot_;
    target -= hotspot_;

    const double vertexPositions[] = {
        origin.x, origin.y,
        target.x, origin.y,
        target.x, target.y,
        origin.x, target.y,
    };

    static const float texturePositions[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    GLuint vertexbuffer;
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturePositions), texturePositions, GL_STATIC_DRAW);


    glBindVertexArray(position_buffer_);

    double M[16];
    modifier.AsMatrix4x4(M);
    float Mf[16];
    for(int i = 0; i < 16; ++i)
        Mf[i] = float(M[i]);

    glColor4dv(effect.color().val);

    GLuint shaderid = VIDEO_MANAGER()->default_shader()->id();
    GLuint matrixpos = glGetUniformLocation(shaderid, "geometry_matrix"),
           texturepod = glGetUniformLocation(shaderid, "texture_id");

    glUniformMatrix4fv(matrixpos, 1, GL_FALSE, Mf);

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture_->gltexture());
    //glBindSampler(0, linearFiltering);
    glUniform1i(texturepod, 0); // Texture unit 0 is for drawable texture

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_);
    glVertexAttribPointer(1, 2, GL_FLOAT , GL_FALSE, 0, 0);

    glDrawArrays(GL_QUADS, 0, 4);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindVertexArray(0);
}

}  // namespace graphic
}  // namespace ugdk
