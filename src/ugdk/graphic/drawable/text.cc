#include <ugdk/graphic/drawable/text.h>

#include <ugdk/config/config.h>
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

Text::Text(const std::wstring& message, freetypeglxx::TextureFont *font) 
    : font_(font), vertex_buffer_(NULL), texture_buffer_(NULL) {
    this->SetMessage(message);
}

Text::Text(const std::vector<std::wstring>& message, freetypeglxx::TextureFont *font) 
    : font_(font), vertex_buffer_(NULL), texture_buffer_(NULL) {
    this->SetMessage(message);
}

void Text::SetMessage(const vector<wstring>& longmessage) {
    delete vertex_buffer_;
    delete texture_buffer_;
    size_ = math::Vector2D();

    num_letters_ = 0;
    for(vector<wstring>::const_iterator it = longmessage.begin(); it != longmessage.end(); ++it)
        num_letters_ += it->size();

    vertex_buffer_ = opengl::VertexBuffer::Create(num_letters_ * 4 * sizeof(freetypeglxx::vec2), 
                                                    GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    texture_buffer_= opengl::VertexBuffer::Create(num_letters_ * 4 * sizeof(freetypeglxx::vec2), 
                                                    GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    math::Vector2D pen;
    size_t buffer_offset = 0;
    for(vector<wstring>::const_iterator it = longmessage.begin(); it != longmessage.end(); ++it) {
        const wstring& message = *it;
        for(size_t i = 0; i < message.size(); ++i ) {
            freetypeglxx::TextureGlyph* glyph = font_->GetGlyph(message[i]);
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
        size_.x = std::max(size_.x, pen.x);
        pen.y += font_->height();
        pen.x = 0.0;
    }
    size_.y = pen.y;
}

void Text::SetMessage(const std::wstring& message) {
    SetMessage(std::vector<std::wstring>(1, message));
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
        
    opengl::ShaderProgram::Use shader_use(
        (VIDEO_MANAGER()->default_shader() == InterfaceShader()) 
            ? InterfaceTextShader() 
            : LightSystemTextShader());

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
    glDrawArrays(GL_QUADS, 0, num_letters_ * 4); // 12*3 indices starting at 0 -> 12 triangles

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
