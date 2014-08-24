#include <ugdk/graphic/drawable/functions.h>

#include <glm/glm.hpp>

#include <ugdk/internal/opengl.h>
#include <ugdk/internal/gltexture.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/text/font.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/util/utf8.h>

#include <texture-font.h>
#include <vec234.h>

namespace ugdk {
namespace graphic {

namespace {
    struct VertexXYUV {
        GLfloat x, y, u, v;
    };
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
    
void DrawSquare(const Geometry& geometry, const VisualEffect& effect, const internal::GLTexture* texture) {
    const glm::mat4& mat = geometry.AsMat4();
    if(mat[3].x > 1 || mat[3].y < -1 ||
        mat[0].x + mat[1].x + mat[3].x < -1 ||
        mat[0].y + mat[1].y + mat[3].y > 1)
        return;
    
    // Use our shader
    opengl::ShaderUse shader_use(graphic::manager()->shaders().current_shader());

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    shader_use.SendGeometry(mat);
    shader_use.SendEffect(effect);

    // Bind our texture in Texture Unit 0
    shader_use.SendTexture(0, texture);

    // 1rst attribute buffer : vertices
    shader_use.SendVertexBuffer(opengl::VertexBuffer::CreateDefault(), opengl::VERTEX, 0);

    // 2nd attribute buffer : UVs
    shader_use.SendVertexBuffer(opengl::VertexBuffer::CreateDefault(), opengl::TEXTURE, 0);

    // Use the 4 vertices to draw a square.
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void DrawTextLine(const Geometry& geometry, const VisualEffect& effect, const Font* font, const std::string& utf8_message) {
    auto ucs_msg = utf8_to_ucs4(utf8_message);

    VertexData data(utf8_message.size() * 4, sizeof(VertexXYUV), false, true);
    {
        VertexData::Mapper mapper(data);
        FillBufferWithText(font, ucs_msg, mapper);
    }
    opengl::VertexBuffer::Bind bind(*data.buffer());

    std::vector<int> first_vector;
    first_vector.reserve(ucs_msg.size());
    for(size_t i = 0; i < ucs_msg.size(); ++i )
        first_vector.push_back(i * 4);
    std::vector<int> size_vector(ucs_msg.size(), 4);
    
    graphic::manager()->shaders().ChangeFlag(Manager::Shaders::IGNORE_TEXTURE_COLOR, true);

    opengl::ShaderUse shader_use(graphic::manager()->shaders().current_shader());
    shader_use.SendGeometry(geometry);
    shader_use.SendEffect(effect);
    shader_use.SendTexture(0, font->freetype_font()->atlas->id);

    shader_use.SendVertexBuffer(data.buffer().get(), opengl::VERTEX,             0, 2, data.vertex_size());
    shader_use.SendVertexBuffer(data.buffer().get(), opengl::TEXTURE, sizeof(vec2), 2, data.vertex_size());

#ifdef UGDK_USING_GLES
    for(size_t i = 0; i < ucs_msg.size(); ++i) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
    }
#else
    glMultiDrawArrays(GL_TRIANGLE_STRIP, first_vector.data(), size_vector.data(), ucs_msg.size());
#endif

    graphic::manager()->shaders().ChangeFlag(Manager::Shaders::IGNORE_TEXTURE_COLOR, false);
}

}  // namespace graphic
}  // namespace ugdk
