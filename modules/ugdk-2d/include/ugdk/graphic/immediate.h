#ifndef UGDK_GRAPHIC_IMMEDIATE_H_
#define UGDK_GRAPHIC_IMMEDIATE_H_

#include <ugdk/graphic.h>
#include <ugdk/action.h>
#include <ugdk/math.h>

namespace ugdk {
namespace graphic {
namespace immediate {

/// Creates, configures and then draws the given texture as a rectangle, using the texture's size.
/// This function focuses ease of use, not performance.
void Rectangle(ugdk::graphic::Canvas& canvas, const math::Vector2D& offset, const GLTexture* texture);

/// Creates, configures and then draws a Rectangle of the given size and given texture.
/// This function focuses ease of use, not performance.
void Rectangle(ugdk::graphic::Canvas& canvas, const math::Vector2D& offset, const math::Vector2D& size, const GLTexture* texture);

/// Creates, configures and then draws a Rectangle using the given atlas and sprite animation frame.
/// This function focuses ease of use, not performance.
void Rectangle(ugdk::graphic::Canvas& canvas, const math::Vector2D& offset, const TextureAtlas* atlas, const action::SpriteAnimationFrame& animation_frame);

} // namespace immediate
} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_IMMEDIATE_H_
