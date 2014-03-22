#include <ugdk/graphic/sprite.h>

#include <ugdk/resource/module.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/drawable/functions.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/opengl/Exception.h>
#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/action/animationplayer.h>

namespace ugdk {
namespace graphic {

using action::SpriteAnimationPlayer;
        
void ApplyPositionOffset(VertexData& data, const math::Vector2D& offset) {
    data.CheckSizes("ApplyPositionOffset", 0, 2 * sizeof(GLfloat));

    opengl::VertexBuffer::Bind bind(*data.buffer());
    opengl::VertexBuffer::Mapper mapper(*data.buffer());

    uint8* ptr = static_cast<uint8*>(mapper.get());
    for (std::size_t i = 0; i < data.num_vertices(); ++i) {
        GLfloat* v = reinterpret_cast<GLfloat*>(ptr + i * data.vertex_size());
        v[0] += GLfloat(offset.x);
        v[1] += GLfloat(offset.y);
    }
}

void SpriteDataSetToGeometry(VertexData& data, const math::Vector2D& position, const math::Vector2D& size, const math::Vector2D& hotspot, const Geometry& geometry) {
    data.CheckSizes("SpriteDataSetToGeometry", 4, 2 * sizeof(GLfloat));

    glm::vec4 top_left(position.x - hotspot.x, position.x - hotspot.y, 0.0, 1.0);
    glm::vec4 bottom_right(top_left.x + size.x, top_left.y + size.y, 0.0, 1.0);

    const glm::mat4& mat = geometry.AsMat4();
    top_left = mat * top_left;
    bottom_right = mat * bottom_right;

    {
        opengl::VertexBuffer::Bind bind(*data.buffer());
        opengl::VertexBuffer::Mapper mapper(*data.buffer());

        uint8* ptr = static_cast<uint8*>(mapper.get());

        GLfloat* v1 = reinterpret_cast<GLfloat*>(ptr + 0 * data.vertex_size());
        v1[0] = float(top_left.x);
        v1[1] = float(top_left.y);

        GLfloat* v2 = reinterpret_cast<GLfloat*>(ptr + 1 * data.vertex_size());
        v2[0] = float(top_left.x);
        v2[1] = float(bottom_right.y);

        GLfloat* v3 = reinterpret_cast<GLfloat*>(ptr + 2 * data.vertex_size());
        v3[2 * 4 + 0] = float(bottom_right.x);
        v3[2 * 4 + 1] = float(top_left.y);

        GLfloat* v4 = reinterpret_cast<GLfloat*>(ptr + 3 * data.vertex_size());
        v4[0] = float(bottom_right.x);
        v4[1] = float(bottom_right.y);
    }
}
        
Sprite* Sprite::Create(const Spritesheet *spritesheet, const action::SpriteAnimationTable* table) {
    return new Sprite(spritesheet, table);
}

Sprite* Sprite::Create(const std::string& spritesheet_tag, const action::SpriteAnimationTable* table) {
    return new Sprite(resource::GetSpritesheetFromTag(spritesheet_tag), table);
}

Sprite* Sprite::Create(const std::string& spritesheet_tag, const std::string& animation_set_tag) {
    return new Sprite(resource::GetSpritesheetFromTag(spritesheet_tag), resource::GetSpriteAnimationTableFromFile(animation_set_tag));
}

Sprite* Sprite::Create(const Spritesheet *spritesheet, const std::string& animation_set_tag) {
    return new Sprite(spritesheet, resource::GetSpriteAnimationTableFromFile(animation_set_tag));
}

Sprite::Sprite(const Spritesheet *spritesheet, const action::SpriteAnimationTable* table) 
: spritesheet_(spritesheet)
, primitive_(new Primitive(nullptr, std::make_shared<VertexData>(4, 2 * 2 * sizeof(GLfloat), true)))
, animation_player_(table)
{}

Sprite::~Sprite() {}

std::shared_ptr<Primitive> Sprite::primitive() const {
    return primitive_;
}
    
const action::SpriteAnimationPlayer& Sprite::animation_player() const { 
    return animation_player_;
}

action::SpriteAnimationPlayer& Sprite::animation_player() { 
    return animation_player_;
}
    
void Sprite::ChangeToFrame(const action::SpriteAnimationFrame& frame) {
    const auto& spritesheet_frame = spritesheet_->frame(frame.spritesheet_frame());

    primitive_->set_texture(spritesheet_frame.texture.get());
    SpriteDataSetToGeometry(*primitive_->vertexdata(), position_, spritesheet_frame.size, spritesheet_frame.hotspot, frame.geometry());
}
    
void Sprite::ChangePosition(const math::Vector2D& position) {
    ApplyPositionOffset(*primitive_->vertexdata(), position - position_);
    position_ = position;
}

}  // namespace graphic
}  // namespace ugdk
