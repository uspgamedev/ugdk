#include <ugdk/graphic/drawable/label.h>

#ifndef UGDK_USING_GLES
#include <texture-font.h>
#include <vec234.h>
#endif

#include <ugdk/internal/opengl.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/defaultshaders.h>

#include <ugdk/structure/types.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/font.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/util/utf8.h>
#include <ugdk/graphic/drawable/functions.h>

#include <algorithm>

namespace ugdk {
namespace graphic {

using ugdk::Color;

Label::Label(const std::string& utf8_message, Font *font) 
    : font_(font), vertex_buffer_(nullptr), texture_buffer_(nullptr) {
    this->ChangeMessage(utf8_message);
}

Label::Label(const std::u32string& ucs4_message, Font *font) 
    : font_(font), vertex_buffer_(nullptr), texture_buffer_(nullptr) {
    this->ChangeMessage(ucs4_message);
}

Label::~Label() {
    delete vertex_buffer_;
    delete texture_buffer_;
    vertex_buffer_ = nullptr;
    texture_buffer_ = nullptr;
}

void Label::ChangeMessage(const std::string& utf8_message) {
    ChangeMessage(utf8_to_ucs4(utf8_message));
}

void Label::ChangeMessage(const std::u32string& ucs4_message) {
    delete vertex_buffer_;
    delete texture_buffer_;
    indices_.clear();
    
#ifndef UGDK_USING_GLES
    num_characters_ = ucs4_message.size();
    size_ = math::Vector2D(0, font_->height());

    vertex_buffer_ = opengl::VertexBuffer::Create(num_characters_ * 4 * sizeof(vec2),
                                                    GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    texture_buffer_= opengl::VertexBuffer::Create(num_characters_ * 4 * sizeof(vec2), 
                                                    GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    vec2 pen = {0.0f, 0.0f};
    size_t buffer_offset = 0;
    for(size_t i = 0; i < ucs4_message.size(); ++i ) {
        texture_glyph_t *glyph = texture_font_get_glyph(font_->freetype_font(),
                                                        static_cast<wchar_t>(ucs4_message[i]));
        if(!glyph) continue;
        float kerning = 0;
        if(i > 0)
            kerning = texture_glyph_get_kerning(glyph, static_cast<wchar_t>( ucs4_message[i-1] ));
        
        pen.x += kerning;
        float x0  = pen.x + glyph->offset_x;
        float y0  = static_cast<float>(glyph->offset_y);
        float x1  = x0 + glyph->width;
        float y1  = y0 - glyph->height;
        y0 = pen.y + font_->freetype_font()->height - y0;
        y1 = pen.y + font_->freetype_font()->height - y1;
        {
            opengl::VertexBuffer::Bind bind(*vertex_buffer_);
            vec2 points[4];
            points[0].x = x0;
            points[0].y = y0;

            points[1].x = x1;
            points[1].y = y0;

            points[2].x = x1;
            points[2].y = y1;

            points[3].x = x0;
            points[3].y = y1;
            vertex_buffer_->fill(buffer_offset * sizeof(vec2), sizeof(points), points);
        }
        {
            opengl::VertexBuffer::Bind bind(*texture_buffer_);
            vec2 points[4];
            points[0].x = glyph->s0;
            points[0].y = glyph->t0;

            points[1].x = glyph->s1;
            points[1].y = glyph->t0;

            points[2].x = glyph->s1;
            points[2].y = glyph->t1;

            points[3].x = glyph->s0;
            points[3].y = glyph->t1;
            texture_buffer_->fill(buffer_offset * sizeof(vec2), sizeof(points), points);
        }

        indices_.push_back(i * 4 + 0);
        indices_.push_back(i * 4 + 1);
        indices_.push_back(i * 4 + 2);
        indices_.push_back(i * 4 + 0);
        indices_.push_back(i * 4 + 2);
        indices_.push_back(i * 4 + 3);

        pen.x += glyph->advance_x;
        buffer_offset += 4;
    }
    size_.x = pen.x;
#else
    size_ = math::Vector2D(200.0, 50.0);
#endif
}

const ugdk::math::Vector2D& Label::size() const {
    return size_;
}

void Label::Draw(Canvas& canvas) const {
#ifndef UGDK_USING_GLES
    canvas.PushAndCompose(Geometry(-hotspot_));
    
    if(draw_setup_function_) draw_setup_function_(this, canvas);

    graphic::manager()->shaders().ChangeFlag(Manager::Shaders::IGNORE_TEXTURE_COLOR, true);
    opengl::ShaderProgram::Use shader_use(graphic::manager()->shaders().current_shader());

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    shader_use.SendGeometry(canvas.current_geometry());
    shader_use.SendEffect(canvas.current_visualeffect());

    // Bind our texture in Texture Unit 0
    shader_use.SendTexture(0, font_->freetype_font()->atlas->id);

    // 1rst attribute buffer : vertices
    shader_use.SendVertexBuffer(vertex_buffer_, opengl::VERTEX, 0);

    // 2nd attribute buffer : UVs
    shader_use.SendVertexBuffer(texture_buffer_, opengl::TEXTURE, 0);

    // Draw the triangle !
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(num_characters_ * 6), GL_UNSIGNED_SHORT, indices_.data()); // 12*3 indices starting at 0 -> 12 triangles
    
    graphic::manager()->shaders().ChangeFlag(Manager::Shaders::IGNORE_TEXTURE_COLOR, false);

    canvas.PopGeometry();
#else
    DrawSquare(canvas.current_geometry() * Geometry(-hotspot_, size_), canvas.current_visualeffect(), manager()->white_texture());
#endif
}

}  // namespace graphic
}  // namespace ugdk
