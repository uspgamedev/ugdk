#include <ugdk/graphic/sprite.h>

#include <ugdk/resource/module.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/drawable/functions.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/action/animationplayer.h>

namespace ugdk {
namespace graphic {

using action::SpriteAnimationPlayer;
        
Sprite::SpriteData::SpriteData()
: position_(opengl::VertexBuffer::Create(sizeof(GLfloat) * 2 * 4, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW))
, uv_(opengl::VertexBuffer::CreateDefaultShared())
{}

Sprite::SpriteData::~SpriteData() {}
        
void Sprite::SpriteData::ApplyPositionOffset(const math::Vector2D& offset) {
    opengl::VertexBuffer::Bind bind(*position_);
    opengl::VertexBuffer::Mapper mapper(*position_);

    GLfloat *indices = static_cast<GLfloat*>(mapper.get());
    if (indices) {
        for (int i = 0; i < 4; ++i) {
            indices[2 * i + 0] += offset.x;
            indices[2 * i + 1] += offset.y;
        }
    }
}

void Sprite::SpriteData::SetToGeometry(const math::Vector2D& position, const math::Vector2D& size, const math::Vector2D& hotspot, const Geometry& geometry) {

    glm::vec4 top_left(position.x - hotspot.x, position.x - hotspot.y, 0.0, 1.0);
    glm::vec4 bottom_right(top_left.x + size.x, top_left.y + size.y, 0.0, 1.0);

    const glm::mat4& mat = geometry.AsMat4();
    top_left = mat * top_left;
    bottom_right = mat * bottom_right;

    GLfloat buffer_data[] = {
        float(top_left.x),     float(top_left.y),
        float(top_left.x),     float(bottom_right.y),
        float(bottom_right.x), float(top_left.y),
        float(bottom_right.x), float(bottom_right.y)
    };
    {
        opengl::VertexBuffer::Bind bind(*position_);
        opengl::VertexBuffer::Mapper mapper(*position_);

        GLfloat *indices = static_cast<GLfloat*>(mapper.get());
        if (indices)
            memcpy(indices, buffer_data, sizeof(buffer_data));
    }
}
        
void Sprite::SpriteData::Draw(opengl::ShaderUse& shader_use) const {
    shader_use.SendVertexBuffer(position_.get(), opengl::VERTEX, 0);
    shader_use.SendVertexBuffer(uv_.get(), opengl::TEXTURE, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
: sprite_data_(new Sprite::SpriteData)
, spritesheet_(spritesheet)
, primitive_(new Primitive(nullptr, sprite_data_))
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
    sprite_data_->SetToGeometry(position_, spritesheet_frame.size, spritesheet_frame.hotspot, frame.geometry());
}
    
void Sprite::ChangePosition(const math::Vector2D& position) {
    sprite_data_->ApplyPositionOffset(position - position_);
    position_ = position;
}

}  // namespace graphic
}  // namespace ugdk
