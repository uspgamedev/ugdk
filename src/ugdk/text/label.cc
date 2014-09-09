#include <ugdk/text/label.h>

#include <ugdk/internal/opengl.h>

#include <ugdk/structure/types.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/text/font.h>
#include <ugdk/util/utf8.h>
#include <ugdk/text/functions.h>

#include <texture-font.h>
#include <vec234.h>

#include <algorithm>

namespace ugdk {
namespace text {

using namespace graphic;
using ugdk::Color;

Label::Label(const std::string& utf8_message, Font *font) 
    : font_(font)
{
    this->ChangeMessage(utf8_message);
}

Label::Label(const std::u32string& ucs4_message, Font *font) 
    : font_(font)
{
    this->ChangeMessage(ucs4_message);
}

Label::~Label() {}

void Label::ChangeMessage(const std::string& utf8_message) {
    ChangeMessage(utf8_to_ucs4(utf8_message));
}

void Label::ChangeMessage(const std::u32string& ucs4_message) {
    buffer_.reset();
    first_vector_.clear();
    size_vector_.clear();
    
    assert(font_);
    
    num_characters_ = ucs4_message.size();
    size_ = math::Vector2D(0, font_->height());

    buffer_.reset(new VertexData(num_characters_ * 4, 2 * sizeof(vec2), false));
    {
        // Leaving the buffer bound may cause texture_font_get_glyph to crash.
        VertexData::Mapper mapper(*buffer_);
        size_.x = FillBufferWithText(font_, ucs4_message, mapper, 0.0f);
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

    auto& shaders = graphic::manager()->shaders();
    bool previous_ignore_texture_flag = shaders.IsFlagSet(graphic::Manager::Shaders::IGNORE_TEXTURE_COLOR);
    auto previous_program = canvas.shader_program();

    shaders.ChangeFlag(graphic::Manager::Shaders::IGNORE_TEXTURE_COLOR, true);
    canvas.ChangeShaderProgram(shaders.current_shader());

    TextureUnit unit = manager()->ReserveTextureUnit(nullptr);
    glActiveTexture(GL_TEXTURE0 + unit.id());
    glBindTexture(GL_TEXTURE_2D, font_->freetype_font()->atlas->id);
    canvas.SendUniform("drawable_texture", unit);

    canvas.SendVertexData(*buffer_, VertexType::VERTEX, 0, 2);
    canvas.SendVertexData(*buffer_, VertexType::TEXTURE, sizeof(vec2), 2);

#ifdef UGDK_USING_GLES
    for (size_t i = 0; i < num_characters_; ++i) {
        glDrawArrays(GL_TRIANGLE_STRIP, static_cast<int>(i * 4), 4);
    }
#else
    glMultiDrawArrays(GL_TRIANGLE_STRIP, first_vector_.data(), size_vector_.data(), static_cast<int>(num_characters_));
#endif

    shaders.ChangeFlag(graphic::Manager::Shaders::IGNORE_TEXTURE_COLOR, previous_ignore_texture_flag);
    canvas.ChangeShaderProgram(previous_program);

    canvas.PopGeometry();
}

}  // namespace text
}  // namespace ugdk
