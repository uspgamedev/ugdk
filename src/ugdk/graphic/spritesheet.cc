#include <ugdk/graphic/spritesheet.h>

#include <ugdk/math/integer2D.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/spritesheetdata.h>
#include <ugdk/internal/pixelsurface.h>
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
        
Spritesheet::Frame::Frame(Texture* _tex, const math::Vector2D& _size, const math::Vector2D& _hotspot)
    : texture(_tex)
    , size(_size)
    , hotspot(_hotspot) {}

Spritesheet::Frame::~Frame() {}

Spritesheet::Frame::Frame(Spritesheet::Frame&& other)
    : texture(std::move(other.texture))
    , size(std::move(other.size))
    , hotspot(std::move(other.hotspot)) {}

Spritesheet::Frame& Spritesheet::Frame::operator=(Spritesheet::Frame&& other) {
    this->texture = std::move(other.texture);
    this->size = std::move(other.size);
    this->hotspot = std::move(other.hotspot);
    return *this;
}
        
Spritesheet::Spritesheet(const SpritesheetData& data)
{
    for(const SpritesheetData::SpritesheetFrame& frame : data.frames()) {
        Texture* texture = Texture::CreateFromSurface(frame.surface->surface);
        frames_.emplace_back(texture, math::Vector2D(math::Integer2D(texture->width(), texture->height())), frame.hotspot);
    }
}

Spritesheet::~Spritesheet() {}

const ugdk::math::Vector2D& Spritesheet::frame_size(size_t frame_number) const {
    static const ugdk::math::Vector2D invalid_size(0.0, 0.0);
    return frame_number < frames_.size() ? frames_[frame_number].size : invalid_size;
}

Spritesheet* CreateSpritesheetFromTag(const std::string& tag) {
    using script::VirtualObj;
    if(tag.size() == 0) return nullptr;

    VirtualObj data = SCRIPT_MANAGER()->LoadModule("spritesheets." + SCRIPT_MANAGER()->ConvertPathToDottedNotation(tag));
    if(!data) return nullptr;

    std::list<std::string> filenames;
    {
        VirtualObj::Vector files = data["file"].value<VirtualObj::Vector>();
        for(VirtualObj::Vector::iterator it = files.begin(); it != files.end(); ++it)
            filenames.push_back(it->value<std::string>());
    }

    SpritesheetData sprite_data(filenames);

    if(data["fill"]) {
        // TODO: make fill a vector of fills.
        VirtualObj::Vector fill = data["fill"].value<VirtualObj::Vector>();
        int width = fill[0].value<int>();
        int height = fill[1].value<int>();
        ugdk::math::Vector2D* hotspot = fill[2].value<ugdk::math::Vector2D*>();
        int file = (fill.size() >= 4) ? fill[4].value<int>() : 0;
        sprite_data.FillWithFramesize(width, height, *hotspot, file);
    }
    if(data["frames"]) {
        VirtualObj::Vector frames = data["frames"].value<VirtualObj::Vector>();
        for(VirtualObj::Vector::iterator it = frames.begin(); it != frames.end(); ++it) {
            VirtualObj::Vector frame = it->value<VirtualObj::Vector>();
            int top_left_x = frame[0].value<int>();
            int top_left_y = frame[1].value<int>();
            int width = frame[2].value<int>();
            int height = frame[3].value<int>();
            ugdk::math::Vector2D* hotspot = frame[4].value<ugdk::math::Vector2D*>();
            int file = (frame.size() >= 6) ? frame[5].value<int>() : 0;
            sprite_data.AddFrame(top_left_x, top_left_y, width, height, *hotspot, file);
        }
    }

    return new Spritesheet(sprite_data);
}

}  // namespace graphic
}  // namespace ugdk
