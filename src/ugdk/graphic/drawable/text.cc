#include <ugdk/graphic/drawable/text.h>

#include <ugdk/config/config.h>
#include "GL/glew.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"
#include <freetype-gl++/texture-font.hpp>
#include <freetype-gl++/vec234.hpp>

#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>

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

static ugdk::math::Vector2D GetStringSize(const std::wstring& string, freetypeglxx::TextureFont* font) {
    double width = 0;
    size_t height = font->GetGlyph(L'\n')->height();
    double kerning = 0;
    for(size_t i = 0; i < string.length(); i++) {
        freetypeglxx::TextureGlyph* glyph = font->GetGlyph(string[i]);
        if(i > 0)
            kerning = glyph->GetKerning( string[i-1] );
        width += kerning + glyph->advance_x();
        height = std::max(height, glyph->height());
    }
    return ugdk::math::Vector2D(width, static_cast<double>(height));
}

Text::Text(const std::wstring& message, freetypeglxx::TextureFont *font) 
    : font_(font), vertex_buffer_(NULL), texture_buffer_(NULL) {
    this->SetMessage(message);
}

Text::Text(const std::vector<std::wstring>& message, freetypeglxx::TextureFont *font) 
    : font_(font), vertex_buffer_(NULL), texture_buffer_(NULL) {
    this->SetMessage(message);
}

void Text::SetMessage(const std::wstring& message) {
    delete vertex_buffer_;
    delete texture_buffer_;
    message_.clear();
    size_ = GetStringSize(message, font_);

    vertex_buffer_ = opengl::VertexBuffer::Create(message.size() * 4 * sizeof(freetypeglxx::vec2), 
                                                    GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    texture_buffer_= opengl::VertexBuffer::Create(message.size() * 4 * sizeof(freetypeglxx::vec2), 
                                                    GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    math::Vector2D pen;
    for(size_t i = 0; i < message.size(); ++i ) {
        freetypeglxx::TextureGlyph* glyph = font_->GetGlyph(message[i]);
        if(!glyph) continue;
        double kerning = 0;
        if(i > 0)
            kerning = glyph->GetKerning( message[i-1] );
        pen.x += kerning;
        double x0  = pen.x + glyph->offset_x();
        double y0  = pen.y + glyph->offset_y();
        double x1  = x0 + glyph->width();
        double y1  = y0 - glyph->height();
        y0 = size_.y - y0;
        y1 = size_.y - y1;
        {
            opengl::VertexBuffer::Bind bind(*vertex_buffer_);
            freetypeglxx::vec2 points[4];
            points[0] = freetypeglxx::vec2(x0, y0);
            points[1] = freetypeglxx::vec2(x1, y0);
            points[2] = freetypeglxx::vec2(x1, y1);
            points[3] = freetypeglxx::vec2(x0, y1);
            vertex_buffer_->fill((i * 4) * sizeof(freetypeglxx::vec2), sizeof(points), points);
        }
        {
            opengl::VertexBuffer::Bind bind(*texture_buffer_);
            freetypeglxx::vec2 points[4];
            points[0] = freetypeglxx::vec2(glyph->s0(),glyph->t0());
            points[1] = freetypeglxx::vec2(glyph->s1(),glyph->t0());
            points[2] = freetypeglxx::vec2(glyph->s1(),glyph->t1());
            points[3] = freetypeglxx::vec2(glyph->s0(),glyph->t1());
            texture_buffer_->fill((i * 4) * sizeof(freetypeglxx::vec2), sizeof(points), points);
        }
        pen.x += glyph->advance_x();
    }

    message_.push_back(message);
    line_height_ = size_.y;
    line_width_.clear();
    line_width_.push_back(size_.x);
}

void Text::SetMessage(const std::vector<std::wstring>& message) {
    SetMessage(message[0]);
    /*delete vertex_buffer_;
    delete texture_buffer_;
    std::vector<std::wstring>::const_iterator it;
    for(it = message.begin(); it != message.end(); ++it) {
        ugdk::math::Vector2D line_size = GetStringSize(*it, font_);
        line_height_ = std::max(line_size.y, line_height_);

        size_.x = std::max(size_.x, line_size.x);
        size_.y += line_size.y;

        line_width_.push_back(line_size.x);
    }
    message_.clear();
    message_ = message;*/
}

const ugdk::math::Vector2D& Text::size() const {
    return size_;
}

void Text::Update(double dt) {}

void Text::Draw(const Geometry& geometry, const VisualEffect& effect) const {
    Geometry final_geometry(geometry);
    final_geometry.Compose(Geometry(-hotspot_));

    static Color FANCY_COLORS[3] = {
        Color(1.000000, 1.000000, 1.000000), // 255, 255, 255
        Color(0.831372, 0.666666, 0.000000), // 212, 170,   0
        Color(0.333333, 0.266666, 0.000000)};//  85,  68,   0
        
    opengl::ShaderProgram::Use shader_use(VIDEO_MANAGER()->default_shader());

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    shader_use.SendGeometry(final_geometry);
    shader_use.SendEffect(effect);

    // Bind our texture in Texture Unit 0
    shader_use.SendTexture(0, font_->atlas()->id());

    // 1rst attribute buffer : vertices
    shader_use.SendVertexBuffer(vertex_buffer_, opengl::VERTEX, 0);

    // 2nd attribute buffer : UVs
    shader_use.SendVertexBuffer(texture_buffer_, opengl::TEXTURE, 0);

    // Draw the triangle !
    glDrawArrays(GL_QUADS, 0, message_[0].size() * 4); // 12*3 indices starting at 0 -> 12 triangles

    /*
    Font::IdentType ident = font_->ident();

    glEnable(GL_TEXTURE_2D);
    glListBase(font_->id());

    glPushMatrix();
    glLoadMatrixf(&modifier.AsMat4()[0][0]);
    
    glTranslated(-hotspot_.x, -hotspot_.y, 0.0);
    
    int fancy_line_number = 0;
    if (font_->IsFancy()) fancy_line_number = 2;
    for(; fancy_line_number >= 0; fancy_line_number--) {
        glTranslated(-1.0, -1.0, 0);
        glPushMatrix();

        Color color = effect.color() * FANCY_COLORS[fancy_line_number];
        glColor4dv(color.val);

        for(size_t i = 0; i < message_.size(); ++i) {
            if(i != 0) glTranslated( 0.0, line_height_, 0.0);
            if(message_[i].length() > 0) {
                glPushMatrix();
                switch(ident) {
                    case Font::CENTER:
                        glTranslated((size_.x - this->line_width_[i])*0.5, 0.0, 0.0);
                        break;
                    case Font::RIGHT:
                        glTranslated((size_.x - this->line_width_[i])*1.0, 0.0, 0.0);
                        break;
                    default:
                        break;
                }
                #ifdef WIN32
                    glCallLists(static_cast<GLsizei>(message_[i].length()), GL_SHORT, (GLshort *) message_[i].c_str());
                #else
                    glCallLists(static_cast<GLsizei>(message_[i].length()), GL_INT, (GLint *) message_[i].c_str());
                #endif
                glPopMatrix();
            }
        }
        glPopMatrix();
    }
    glPopMatrix();*/
}

}  // namespace graphic
}  // namespace ugdk
