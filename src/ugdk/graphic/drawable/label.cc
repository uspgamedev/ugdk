#include <ugdk/graphic/drawable/label.h>

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

#include <texture-font.h>
#include <vec234.h>

#include <algorithm>

namespace ugdk {
namespace graphic {

using ugdk::Color;

Label::Label(const std::string& utf8_message, Font *font) 
    : font_(font), buffer_(nullptr) {
    this->ChangeMessage(utf8_message);
}

Label::Label(const std::u32string& ucs4_message, Font *font) 
    : font_(font), buffer_(nullptr) {
    this->ChangeMessage(ucs4_message);
}

Label::~Label() {
    delete buffer_;
    buffer_ = nullptr;
}

void Label::ChangeMessage(const std::string& utf8_message) {
    ChangeMessage(utf8_to_ucs4(utf8_message));
}

void Label::ChangeMessage(const std::u32string& ucs4_message) {
    delete buffer_;
    first_vector_.clear();
    size_vector_.clear();
    
    assert(font_);
    
    num_characters_ = ucs4_message.size();
    size_ = math::Vector2D(0, font_->height());

    buffer_ = opengl::VertexBuffer::Create(num_characters_ * 8 * sizeof(vec2), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    // Leaving the buffer bound may cause texture_font_get_glyph to crash.
    opengl::VertexBuffer::Bind bind(*buffer_);
    opengl::VertexBuffer::Mapper mapper(*buffer_);
    auto buf = static_cast<vec2*>(mapper.get());

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
            buf[buffer_offset + 0].x = x0;
            buf[buffer_offset + 0].y = y0;
            buf[buffer_offset + 1].x = glyph->s0;
            buf[buffer_offset + 1].y = glyph->t0;

            buf[buffer_offset + 2].x = x1;
            buf[buffer_offset + 2].y = y0;
            buf[buffer_offset + 3].x = glyph->s1;
            buf[buffer_offset + 3].y = glyph->t0;

            buf[buffer_offset + 4].x = x0;
            buf[buffer_offset + 4].y = y1;
            buf[buffer_offset + 5].x = glyph->s0;
            buf[buffer_offset + 5].y = glyph->t1;

            buf[buffer_offset + 6].x = x1;
            buf[buffer_offset + 6].y = y1;
            buf[buffer_offset + 7].x = glyph->s1;
            buf[buffer_offset + 7].y = glyph->t1;

            buffer_offset += 8;
        }
        pen.x += glyph->advance_x;
    }
    size_.x = pen.x;

    first_vector_.reserve(ucs4_message.size());
    for(size_t i = 0; i < ucs4_message.size(); ++i )
        first_vector_.push_back(i * 4);
    size_vector_.resize(ucs4_message.size(), 4);
}

const ugdk::math::Vector2D& Label::size() const {
    return size_;
}

void Label::Draw(Canvas& canvas) const {
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
    shader_use.SendVertexBuffer(buffer_, opengl::VERTEX,             0, 2, 2 * sizeof(vec2));

    // 2nd attribute buffer : UVs
    shader_use.SendVertexBuffer(buffer_, opengl::TEXTURE, sizeof(vec2), 2, 2 * sizeof(vec2));

    // Draw the line!
    glMultiDrawArrays(GL_TRIANGLE_STRIP, first_vector_.data(), size_vector_.data(), num_characters_);
    
    graphic::manager()->shaders().ChangeFlag(Manager::Shaders::IGNORE_TEXTURE_COLOR, false);

    canvas.PopGeometry();
}

}  // namespace graphic
}  // namespace ugdk
