#include <ugdk/graphic/sprite.h>

#include <ugdk/resource/module.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/drawable/functions.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/opengl/vertexdata_rectangle.h>
#include <ugdk/graphic/opengl/Exception.h>
#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/action/animationplayer.h>

namespace ugdk {
namespace graphic {

using action::SpriteAnimationPlayer;

namespace {
    struct VertexXY {
        GLfloat x, y;
    };
    struct VertexXYUV {
        GLfloat x, y, u, v;
    };
}
        
void ApplyPositionOffset(VertexData& data, const math::Vector2D& offset) {
    data.CheckSizes("ApplyPositionOffset", 0, sizeof(VertexXY));

    opengl::VertexBuffer::Bind bind(*data.buffer());
    opengl::VertexBuffer::Mapper mapper(*data.buffer());

    uint8* ptr = static_cast<uint8*>(mapper.get());
    for (std::size_t i = 0; i < data.num_vertices(); ++i) {
        VertexXY* v = reinterpret_cast<VertexXY*>(ptr + i * data.vertex_size());
        v->x += GLfloat(offset.x);
        v->y += GLfloat(offset.y);
    }
}

void SpriteDataSetToGeometry(VertexData& data, const math::Vector2D& position, const action::SpriteAnimationFrame& animation_frame, const Spritesheet::Frame& spritesheet_frame) {
    data.CheckSizes("SpriteDataSetToGeometry", 4, sizeof(VertexXYUV));

    math::Vector2D mirror_scale(
        (animation_frame.mirror() & ugdk::MIRROR_HFLIP) ? -1.0 : 1.0,
        (animation_frame.mirror() & ugdk::MIRROR_VFLIP) ? -1.0 : 1.0);

    Geometry final_transform =
        animation_frame.geometry()
        * Geometry(position, mirror_scale)
        * Geometry(-spritesheet_frame.hotspot);

    const glm::mat4& mat = final_transform.AsMat4();
    glm::vec4 top_left = mat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 bottom_right = mat * glm::vec4(spritesheet_frame.piece.size().x, spritesheet_frame.piece.size().y, 0.0, 1.0);

    {
        VertexData::Mapper mapper(data);

        VertexXYUV* v1 = mapper.Get<VertexXYUV>(0);
        v1->x = float(top_left.x);
        v1->y = float(top_left.y);
        spritesheet_frame.piece.ConvertToAtlas(0.0f, 0.0f, &v1->u, &v1->v);

        VertexXYUV* v2 = mapper.Get<VertexXYUV>(1);
        v2->x = float(top_left.x);
        v2->y = float(bottom_right.y);
        spritesheet_frame.piece.ConvertToAtlas(0.0f, 1.0f, &v2->u, &v2->v);

        VertexXYUV* v3 = mapper.Get<VertexXYUV>(2);
        v3->x = float(bottom_right.x);
        v3->y = float(top_left.y);
        spritesheet_frame.piece.ConvertToAtlas(1.0f, 0.0f, &v3->u, &v3->v);

        VertexXYUV* v4 = mapper.Get<VertexXYUV>(3);
        v4->x = float(bottom_right.x);
        v4->y = float(bottom_right.y);
        spritesheet_frame.piece.ConvertToAtlas(1.0f, 1.0f, &v4->u, &v4->v);
    }
}

Sprite::Sprite(const Spritesheet *spritesheet)
: spritesheet_(spritesheet)
, owner_(nullptr)
{}

Sprite::~Sprite() {}

void Sprite::set_owner(Primitive* owner) {
    owner_ = owner;
}

void Sprite::ChangeToFrame(const action::SpriteAnimationFrame& frame) {
    const auto& spritesheet_frame = spritesheet_->frame(frame.spritesheet_frame());

    owner_->set_visualeffect(frame.effect());
    SpriteDataSetToGeometry(*owner_->vertexdata(), position_, frame, spritesheet_frame);
}
    
void Sprite::ChangePosition(const math::Vector2D& position) {
    ApplyPositionOffset(*owner_->vertexdata(), position - position_);
    position_ = position;
}

std::shared_ptr<VertexData> CreateSpriteCompatibleVertexData() {
    return std::make_shared<VertexData>(4, sizeof(VertexXYUV), true);
}

void PreparePrimitiveForSprite(Primitive& primitive, const Spritesheet *spritesheet) {
    primitive.set_texture(spritesheet->atlas()->texture());
    primitive.set_vertexdata(CreateSpriteCompatibleVertexData());
    primitive.set_controller(std::unique_ptr<Sprite>(new Sprite(spritesheet)));
    primitive.set_drawfunction(ugdk::graphic::opengl::RenderPrimitiveAsRectangle);
}

std::shared_ptr<action::SpriteAnimationPlayer> CreateSpriteAnimationPlayerForPrimitive(Primitive& primitive, const action::SpriteAnimationTable* table) {
    Primitive* p = &primitive;
    std::shared_ptr<action::SpriteAnimationPlayer> player(new action::SpriteAnimationPlayer(table));
    player->set_frame_change_callback([p](const action::SpriteAnimationFrame& frame) {
        if (Sprite* sprite = dynamic_cast<Sprite*>(p->controller().get())) {
            sprite->ChangeToFrame(frame);
        }
    });
    return player;
}

}  // namespace graphic
}  // namespace ugdk
