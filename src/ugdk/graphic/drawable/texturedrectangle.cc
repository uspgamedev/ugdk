#include <ugdk/config/config.h>
#include "GL/glew.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"

#include <ugdk/graphic/drawable/texturedrectangle.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/texture.h>

namespace ugdk {
namespace graphic {

TexturedRectangle::TexturedRectangle(Texture* texture) 
    :   size_(static_cast<double>(texture->width()), static_cast<double>(texture->height())), 
        texture_(texture) {
    //glGenBuffers(1, &position_buffer_);
}

TexturedRectangle::TexturedRectangle(Texture* texture, const ugdk::math::Vector2D& size) 
    : size_(size), texture_(texture) {
    //glGenBuffers(1, &position_buffer_);
}

TexturedRectangle::~TexturedRectangle() {
    //glDeleteBuffers(1, &position_buffer_);
}

void TexturedRectangle::Update(double dt) {}

void TexturedRectangle::Draw(const Geometry& modifier, const VisualEffect& effect) const {
    ugdk::math::Vector2D origin, target(size_);

    origin -= hotspot_;
    target -= hotspot_;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_->gltexture());

    const float vertexPositions[] = {
        origin.x, origin.y, 0.0f, 1.0f,
        target.x, origin.y, 0.0f, 1.0f,
        target.x, target.y, 0.0f, 1.0f,
        origin.x, target.y, 0.0f, 1.0f,
    };

    static const float texturePositions[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    //glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    //glDisableVertexAttribArray(0);

    double M[16];
    modifier.AsMatrix4x4(M);
    glPushMatrix();
    glLoadMatrixd(M);

    glColor4dv(effect.color().val);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(4, GL_FLOAT, 4*sizeof(float), vertexPositions);
    glTexCoordPointer(2, GL_FLOAT, 2*sizeof(float), texturePositions);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}

}  // namespace graphic
}  // namespace ugdk
