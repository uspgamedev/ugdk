#include <ugdk/graphic/spritesheet.h>

#include <ugdk/math/integer2D.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/internal/gltexture.h>
#include <ugdk/internal/pixelsurface.h>
#include <ugdk/resource/module.h>
#include <ugdk/system/engine.h>

#include "SDL_image.h"
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>

#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>

namespace ugdk {
namespace graphic {
        
Spritesheet::Frame::Frame(const TextureAtlas& atlas, size_t index, const math::Vector2D& _hotspot)
: piece(atlas.PieceAt(index))
, hotspot(_hotspot) {}

Spritesheet::Frame::~Frame() {}

Spritesheet::Spritesheet(const std::shared_ptr<TextureAtlas>& atlas, const std::vector< std::pair<size_t, math::Vector2D> >& frames)
: atlas_(atlas)
{
    frames_.reserve(frames.size());
    for (const auto& frame : frames) {
        frames_.emplace_back(*atlas_, frame.first, frame.second);
    }
}

Spritesheet::~Spritesheet() {}

const ugdk::math::Vector2D& Spritesheet::frame_size(size_t frame_number) const {
    static const ugdk::math::Vector2D invalid_size(0.0, 0.0);
    return frame_number < frames_.size() ? frames_[frame_number].piece.size() : invalid_size;
}

Spritesheet* CreateSpritesheetFromTag(const std::string& tag) {
    using script::VirtualObj;
    if(tag.size() == 0) return nullptr;

    VirtualObj data = SCRIPT_MANAGER()->LoadModule("spritesheets." + SCRIPT_MANAGER()->ConvertPathToDottedNotation(tag));
    if(!data) return nullptr;

    std::string filename = data["file"].value<VirtualObj::Vector>()[0].value<std::string>();
    auto texture = ugdk::resource::GetTextureFromFile(filename);

    std::vector< std::pair<math::Vector2D, math::Vector2D> > textureatlas_frames;
    std::vector< std::pair<size_t, math::Vector2D> > spritesheet_frames;

    if (data["fill"]) {
        // TODO: make fill a vector of fills.
        VirtualObj::Vector fill = data["fill"].value<VirtualObj::Vector>();
        int width = fill[0].value<int>();
        int height = fill[1].value<int>();
        ugdk::math::Vector2D* hotspot = fill[2].value<ugdk::math::Vector2D*>();

        int i = 0;
        for (int y = 0; y + height <= texture->height(); y += height) {
            for (int x = 0; x + width <= texture->width(); x += width) {
                textureatlas_frames.emplace_back(math::Vector2D(x, y), math::Vector2D(width, height));
                spritesheet_frames.emplace_back(i++, *hotspot);
            }
        }
    }
    if (data["frames"]) {
        VirtualObj::Vector frames = data["frames"].value<VirtualObj::Vector>();
        int i = 0;
        for (VirtualObj::Vector::iterator it = frames.begin(); it != frames.end(); ++it) {
            VirtualObj::Vector frame = it->value<VirtualObj::Vector>();
            int top_left_x = frame[0].value<int>();
            int top_left_y = frame[1].value<int>();
            int width = frame[2].value<int>();
            int height = frame[3].value<int>();
            ugdk::math::Vector2D* hotspot = frame[4].value<ugdk::math::Vector2D*>();

            textureatlas_frames.emplace_back(math::Vector2D(top_left_x, top_left_y), math::Vector2D(width, height));
            spritesheet_frames.emplace_back(i++, *hotspot);
        }
    }

    std::shared_ptr<TextureAtlas> atlas(new TextureAtlas(texture, textureatlas_frames.size()));

    int i = 0;
    for (const auto& textureatlas_frame : textureatlas_frames) {
        atlas->AddPiece(std::to_string(i++), textureatlas_frame.first, textureatlas_frame.second);
    }
    return new Spritesheet(atlas, spritesheet_frames);
}

}  // namespace graphic
}  // namespace ugdk
