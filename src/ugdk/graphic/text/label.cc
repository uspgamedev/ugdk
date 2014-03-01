#include <ugdk/graphic/text/label.h>

#include <ugdk/internal/opengl.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/defaultshaders.h>

#include <ugdk/structure/types.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/text/font.h>
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
    {
        opengl::VertexBuffer::Bind bind(*buffer_);
        opengl::VertexBuffer::Mapper mapper(*buffer_);
        // Leaving the buffer bound may cause texture_font_get_glyph to crash.
        size_.x = FillBufferWithText(font_, ucs4_message, mapper.get(), 0.0f);
    }

    first_vector_.reserve(num_characters_);
    for(size_t i = 0; i < num_characters_; ++i )
        first_vector_.push_back(int(i) * 4);
    size_vector_.resize(num_characters_, 4);
}

const ugdk::math::Vector2D& Label::size() const {
    return size_;
}

void Label::Draw(Canvas& canvas) const {
    canvas.PushAndCompose(Geometry(-hotspot_));
    
    if(draw_setup_function_) draw_setup_function_(this, canvas);

    graphic::manager()->shaders().ChangeFlag(Manager::Shaders::IGNORE_TEXTURE_COLOR, true);
    opengl::ShaderUse shader_use(graphic::manager()->shaders().current_shader());

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
#ifdef UGDK_USING_GLES
    for(size_t i = 0; i < num_characters_; ++i) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
    }
#else
    glMultiDrawArrays(GL_TRIANGLE_STRIP, first_vector_.data(), size_vector_.data(), num_characters_);
#endif
    
    graphic::manager()->shaders().ChangeFlag(Manager::Shaders::IGNORE_TEXTURE_COLOR, false);

    canvas.PopGeometry();
}

}  // namespace graphic
}  // namespace ugdk
