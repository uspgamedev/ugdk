#include <ugdk/graphic/drawable/label.h>

#include <ugdk/config/config.h>
#include <algorithm>
#include "GL/glew.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"
#include <freetype-gl++/texture-font.hpp>
#include <freetype-gl++/vec234.hpp>

#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/defaultshaders.h>

#include <ugdk/base/engine.h>
#include <ugdk/base/types.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/math/frame.h>
#include <ugdk/graphic/font.h>

namespace ugdk {
namespace graphic {

using ugdk::Color;
using std::vector;
using std::wstring;

Label::Label(const std::wstring& message, Font *font) 
    : font_(font), vertex_buffer_(NULL), texture_buffer_(NULL) {
    this->ChangeMessage(message);
}

Label::~Label() {
    delete vertex_buffer_;
    delete texture_buffer_;
    vertex_buffer_ = NULL;
    texture_buffer_ = NULL;
}

void Label::ChangeMessage(const std::wstring& message) {
    delete vertex_buffer_;
    delete texture_buffer_;
    message_ = message;
    size_ = math::Vector2D(0, font_->height());

    vertex_buffer_ = opengl::VertexBuffer::Create(message_.size() * 4 * sizeof(freetypeglxx::vec2),
                                                    GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    texture_buffer_= opengl::VertexBuffer::Create(message_.size() * 4 * sizeof(freetypeglxx::vec2), 
                                                    GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    math::Vector2D pen;
    size_t buffer_offset = 0;
    for(size_t i = 0; i < message.size(); ++i ) {
        freetypeglxx::TextureGlyph* glyph = font_->freetype_font()->GetGlyph(message[i]);
        if(!glyph) continue;
        double kerning = 0;
        if(i > 0)
            kerning = glyph->GetKerning( message[i-1] );
        pen.x += kerning;
        double x0  = pen.x + glyph->offset_x();
        double y0  = glyph->offset_y();
        double x1  = x0 + glyph->width();
        double y1  = y0 - glyph->height();
        y0 = pen.y + font_->height() - y0;
        y1 = pen.y + font_->height() - y1;
        {
            opengl::VertexBuffer::Bind bind(*vertex_buffer_);
            freetypeglxx::vec2 points[4];
            points[0] = freetypeglxx::vec2(x0, y0);
            points[1] = freetypeglxx::vec2(x1, y0);
            points[2] = freetypeglxx::vec2(x1, y1);
            points[3] = freetypeglxx::vec2(x0, y1);
            vertex_buffer_->fill(buffer_offset * sizeof(freetypeglxx::vec2), sizeof(points), points);
        }
        {
            opengl::VertexBuffer::Bind bind(*texture_buffer_);
            freetypeglxx::vec2 points[4];
            points[0] = freetypeglxx::vec2(glyph->s0(),glyph->t0());
            points[1] = freetypeglxx::vec2(glyph->s1(),glyph->t0());
            points[2] = freetypeglxx::vec2(glyph->s1(),glyph->t1());
            points[3] = freetypeglxx::vec2(glyph->s0(),glyph->t1());
            texture_buffer_->fill(buffer_offset * sizeof(freetypeglxx::vec2), sizeof(points), points);
        }
        pen.x += glyph->advance_x();
        buffer_offset += 4;
    }
    size_.x = pen.x;
}

const ugdk::math::Vector2D& Label::size() const {
    return size_;
}

void Label::Draw(const Geometry& geometry, const VisualEffect& effect) const {
    Geometry final_geometry(geometry);
    final_geometry.Compose(Geometry(-hotspot_));
    
    if(draw_setup_function_) draw_setup_function_(this, geometry, effect);

    VIDEO_MANAGER()->shaders().ChangeFlag(VideoManager::Shaders::IGNORE_TEXTURE_COLOR, true);
    opengl::ShaderProgram::Use shader_use(VIDEO_MANAGER()->shaders().current_shader());

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    shader_use.SendGeometry(final_geometry);
    shader_use.SendEffect(effect);

    // Bind our texture in Texture Unit 0
    shader_use.SendTexture(0, font_->freetype_font()->atlas()->id());

    // 1rst attribute buffer : vertices
    shader_use.SendVertexBuffer(vertex_buffer_, opengl::VERTEX, 0);

    // 2nd attribute buffer : UVs
    shader_use.SendVertexBuffer(texture_buffer_, opengl::TEXTURE, 0);

    // Draw the triangle !
    glDrawArrays(GL_QUADS, 0, message_.size() * 4); // 12*3 indices starting at 0 -> 12 triangles
    
    VIDEO_MANAGER()->shaders().ChangeFlag(VideoManager::Shaders::IGNORE_TEXTURE_COLOR, false);
}

}  // namespace graphic
}  // namespace ugdk
