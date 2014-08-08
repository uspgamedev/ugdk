#include <ugdk/graphic/primitivesetup.h>

#include <ugdk/internal/opengl.h>
#include <ugdk/internal/gltexture.h>
#include <ugdk/graphic/sprite.h>
#include <ugdk/graphic/opengl/shaderuse.h>

namespace ugdk {
namespace graphic {

namespace {
    struct VertexXY {
        GLfloat x, y;
    };
    struct VertexXYUV {
        GLfloat x, y, u, v;
    };

    void RenderPrimitiveAsRectangle(const Primitive& primitive, opengl::ShaderUse& shader_use) {
        auto data = primitive.vertexdata();
        shader_use.SendVertexBuffer(data->buffer().get(), opengl::VERTEX, 0, 2, data->vertex_size());
        shader_use.SendVertexBuffer(data->buffer().get(), opengl::TEXTURE, 2 * sizeof(GLfloat), 2, data->vertex_size());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

namespace VertexDataManipulation {

void ApplyPositionOffset(VertexData& data, const math::Vector2D& offset) {
    data.CheckSizes("ApplyPositionOffset", 0, sizeof(VertexXY));

    VertexData::Mapper mapper(data);
    for (std::size_t i = 0; i < data.num_vertices(); ++i) {
        VertexXY* v = mapper.Get<VertexXY>(i);
        v->x += GLfloat(offset.x);
        v->y += GLfloat(offset.y);
    }
}

void SetToRectangleAtOrigin(VertexData& data, const math::Vector2D& size) {
    data.CheckSizes("PrepareVertexDataAsRectangle", 4, sizeof(VertexXYUV));

    VertexData::Mapper mapper(data);

    VertexXYUV* v1 = mapper.Get<VertexXYUV>(0);
    v1->x = 0.0f;
    v1->y = 0.0f;
    v1->u = 0.0f;
    v1->v = 0.0f;

    VertexXYUV* v2 = mapper.Get<VertexXYUV>(1);
    v2->x = 0.0f;
    v2->y = float(size.y);
    v2->u = 0.0f;
    v2->v = 1.0f;

    VertexXYUV* v3 = mapper.Get<VertexXYUV>(2);
    v3->x = float(size.x);
    v3->y = 0.0f;
    v3->u = 1.0f;
    v3->v = 0.0f;

    VertexXYUV* v4 = mapper.Get<VertexXYUV>(3);
    v4->x = float(size.x);
    v4->y = float(size.y);
    v4->u = 1.0f;
    v4->v = 1.0f;
}

void SetToAbsoluteRectangleWithAtlasPiece(VertexData& data, 
                                          const glm::vec4& top_left,
                                          const glm::vec4& bottom_right,
                                          const TextureAtlas::BoundPiece& piece) {

    data.CheckSizes("VertexDataSet", 4, sizeof(VertexXYUV));

    VertexData::Mapper mapper(data);

    VertexXYUV* v1 = mapper.Get<VertexXYUV>(0);
    v1->x = top_left.x;
    v1->y = top_left.y;
    piece.ConvertToAtlas(0.0f, 0.0f, &v1->u, &v1->v);

    VertexXYUV* v2 = mapper.Get<VertexXYUV>(1);
    v2->x = top_left.x;
    v2->y = bottom_right.y;
    piece.ConvertToAtlas(0.0f, 1.0f, &v2->u, &v2->v);

    VertexXYUV* v3 = mapper.Get<VertexXYUV>(2);
    v3->x = bottom_right.x;
    v3->y = top_left.y;
    piece.ConvertToAtlas(1.0f, 0.0f, &v3->u, &v3->v);

    VertexXYUV* v4 = mapper.Get<VertexXYUV>(3);
    v4->x = bottom_right.x;
    v4->y = bottom_right.y;
    piece.ConvertToAtlas(1.0f, 1.0f, &v4->u, &v4->v);
}

void SetUsingSpriteFrameInformation(VertexData& data, 
                                    const math::Vector2D& position, 
                                    const action::SpriteAnimationFrame& animation_frame,
                                    const TextureAtlas::BoundPiece& piece) {
    math::Vector2D mirror_scale(
        (animation_frame.mirror() & ugdk::MIRROR_HFLIP) ? -1.0 : 1.0,
        (animation_frame.mirror() & ugdk::MIRROR_VFLIP) ? -1.0 : 1.0);

    math::Vector2D mirror_offset(
        (animation_frame.mirror() & ugdk::MIRROR_HFLIP) ? piece.size().x : 0.0,
        (animation_frame.mirror() & ugdk::MIRROR_VFLIP) ? piece.size().y : 0.0);

    Geometry final_transform =
        animation_frame.geometry()
        * Geometry(position + mirror_offset, mirror_scale);

    const glm::mat4& mat = final_transform.AsMat4();
    glm::vec4 top_left = mat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 bottom_right = mat * glm::vec4(piece.size().x, piece.size().y, 0.0, 1.0);

    SetToAbsoluteRectangleWithAtlasPiece(data, top_left, bottom_right, piece);
}

} // namespace VertexDataManipulation

namespace PrimitiveSetup {
                  
    //========================
    // Sprite
    //========================
    const VertexDataSpecification Sprite::vertexdata_specification(4, sizeof(VertexXYUV), true);

    void Sprite::Prepare(Primitive& primitive, const TextureAtlas* spritesheet) {
        primitive.set_texture(spritesheet->texture());
        primitive.set_vertexdata(CreateVertexDataWithSpecification(Sprite::vertexdata_specification));
        primitive.set_controller(std::unique_ptr<PrimitiveControllerSprite>(new PrimitiveControllerSprite(spritesheet)));
        primitive.set_drawfunction(Sprite::Render);
    }
    
    void Sprite::Render(const Primitive& primitive, opengl::ShaderUse& shader_use) {
        RenderPrimitiveAsRectangle(primitive, shader_use);
    }

    std::shared_ptr<action::SpriteAnimationPlayer> Sprite::CreateSpriteAnimationPlayer(
            Primitive& primitive, const action::SpriteAnimationTable* table) {
        Primitive* p = &primitive;
        std::shared_ptr<action::SpriteAnimationPlayer> player(new action::SpriteAnimationPlayer(table));
        player->set_frame_change_callback([p](const action::SpriteAnimationFrame& frame) {
            if (PrimitiveControllerSprite* sprite = dynamic_cast<PrimitiveControllerSprite*>(p->controller().get())) {
                sprite->ChangeToAnimationFrame(frame);
            }
        });
        return player;
    }

    //========================
    // Rectangle
    //========================
    const VertexDataSpecification Rectangle::vertexdata_specification(4, sizeof(VertexXYUV), false);

    void Rectangle::Prepare(Primitive& primitive, const internal::GLTexture* texture,
                            const math::Vector2D& size) {
        primitive.set_texture(texture);
        primitive.set_vertexdata(CreateVertexDataWithSpecification(Rectangle::vertexdata_specification));
        primitive.set_controller(std::unique_ptr<PrimitiveController>(new PrimitiveController));
        primitive.set_drawfunction(Rectangle::Render);

        VertexDataManipulation::SetToRectangleAtOrigin(*primitive.vertexdata(), size);
    }

    void Rectangle::Prepare(Primitive& primitive, const internal::GLTexture* texture) {
        Rectangle::Prepare(primitive, texture, math::Vector2D(texture->width(), texture->height()));
    }

    void Rectangle::Render(const Primitive& primitive, opengl::ShaderUse& shader_use) {
        RenderPrimitiveAsRectangle(primitive, shader_use);
    }

} // namespace PrimitiveSetup


} // namespace graphic
} // namespace ugdk
