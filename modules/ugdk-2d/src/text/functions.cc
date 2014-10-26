#include <ugdk/text/functions.h>

#include <glm/glm.hpp>

#include <ugdk/graphic/opengl.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/textureunit.h>
#include <ugdk/graphic/shaderprogram.h>
#include <ugdk/text/font.h>
#include <ugdk/util/utf8.h>

#include <texture-font.h>
#include <vec234.h>

namespace ugdk {
namespace text {

using namespace graphic;

namespace {
    struct VertexXYUV {
        GLfloat x, y, u, v;
    };
    std::unique_ptr<VertexData> square_data_;
}

float FillBufferWithText(const Font* font, const std::u32string& msg, VertexData::Mapper& mapped_data, float y) {
    float pen = 0.0f;
    size_t buffer_offset = 0;
    for(size_t i = 0; i < msg.size(); ++i ) {
        texture_glyph_t *glyph = texture_font_get_glyph(font->freetype_font(),
                                                        static_cast<wchar_t>(msg[i]));
        if(!glyph) continue;
        float kerning = 0;
        if(i > 0)
            kerning = texture_glyph_get_kerning(glyph, static_cast<wchar_t>( msg[i-1] ));
        
        pen += kerning;
        float x0  = pen + glyph->offset_x;
        float y0  = static_cast<float>(glyph->offset_y);
        float x1  = x0 + glyph->width;
        float y1  = y0 - glyph->height;
        y0 = y + font->freetype_font()->height - y0;
        y1 = y + font->freetype_font()->height - y1;
        {
            VertexXYUV* v1 = mapped_data.Get<VertexXYUV>(buffer_offset + 0);
            v1->x = x0;
            v1->y = y0;
            v1->u = glyph->s0;
            v1->v = glyph->t0;

            VertexXYUV* v2 = mapped_data.Get<VertexXYUV>(buffer_offset + 1);
            v2->x = x1;
            v2->y = y0;
            v2->u = glyph->s1;
            v2->v = glyph->t0;

            VertexXYUV* v3 = mapped_data.Get<VertexXYUV>(buffer_offset + 2);
            v3->x = x0;
            v3->y = y1;
            v3->u = glyph->s0;
            v3->v = glyph->t1;

            VertexXYUV* v4 = mapped_data.Get<VertexXYUV>(buffer_offset + 3);
            v4->x = x1;
            v4->y = y1;
            v4->u = glyph->s1;
            v4->v = glyph->t1;

            buffer_offset += 4;
        }
        pen += glyph->advance_x;
    }
    return pen;
}
    
void DrawTextLine(Canvas& canvas, const Font* font, const std::string& utf8_message) {
    auto ucs_msg = utf8_to_ucs4(utf8_message);

    VertexData data(utf8_message.size() * 4, sizeof(VertexXYUV), false, true);
    {
        VertexData::Mapper mapper(data);
        FillBufferWithText(font, ucs_msg, mapper);
    }

    std::vector<int> first_vector;
    first_vector.reserve(ucs_msg.size());
    for(size_t i = 0; i < ucs_msg.size(); ++i )
        first_vector.push_back(static_cast<int>(i) * 4);
    std::vector<int> size_vector(ucs_msg.size(), 4);

    auto& shaders = graphic::manager()->shaders();
    bool previous_ignore_texture_flag = shaders.IsFlagSet(graphic::Manager::Shaders::IGNORE_TEXTURE_COLOR);
    auto previous_program = canvas.shader_program();
    
    shaders.ChangeFlag(graphic::Manager::Shaders::IGNORE_TEXTURE_COLOR, true);
    canvas.ChangeShaderProgram(shaders.current_shader());

    TextureUnit unit = manager()->ReserveTextureUnit(nullptr);
    glActiveTexture(GL_TEXTURE0 + unit.id());
    glBindTexture(GL_TEXTURE_2D, font->freetype_font()->atlas->id);
    canvas.SendUniform("drawable_texture", unit);

    canvas.SendVertexData(data, VertexType::VERTEX,             0, 2);
    canvas.SendVertexData(data, VertexType::TEXTURE, sizeof(vec2), 2);

#ifdef UGDK_USING_GLES
    for(size_t i = 0; i < ucs_msg.size(); ++i) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
    }
#else
    glMultiDrawArrays(GL_TRIANGLE_STRIP, first_vector.data(), size_vector.data(), static_cast<int>(ucs_msg.size()));
#endif

    shaders.ChangeFlag(graphic::Manager::Shaders::IGNORE_TEXTURE_COLOR, previous_ignore_texture_flag);
    canvas.ChangeShaderProgram(previous_program);
}

}  // namespace text
}  // namespace ugdk
