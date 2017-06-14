#include <ugdk/graphic/textureatlas.h>

#include <ugdk/filesystem/module.h>
#include <ugdk/filesystem/file.h>

#include "gltexture.h"
#include <libjson.h>

namespace ugdk {
namespace graphic {

void TextureAtlas::BoundPiece::ConvertToAtlas(float *u, float *v) const {
    ConvertToAtlas(*u, *v, u, v);
}

void TextureAtlas::BoundPiece::ConvertToAtlas(float in_u, float in_v, float *out_u, float *out_v) const {
    int size_x = piece_->trimmed_size.x;
    int size_y = piece_->trimmed_size.y;
    if (piece_->rotated_90_clockwise) {
        in_v = 1.0f - in_v;
        std::swap(in_u, in_v);
        std::swap(size_x, size_y);
    }
    if (piece_->horizontal_flip) {
        in_u = 1.0f - in_u;
    }
    if (piece_->vertical_flip) {
        in_v = 1.0f - in_v;
    }
    *out_u = (piece_->position.x + (in_u) * size_x) / atlas_->texture_->width();
    *out_v = (piece_->position.y + (in_v) * size_y) / atlas_->texture_->height();
}

TextureAtlas::TextureAtlas(const graphic::GLTexture* texture, std::size_t size)
:   texture_(texture)
{
    pieces_.reserve(size);
}

TextureAtlas::~TextureAtlas() {}

TextureAtlas* TextureAtlas::LoadFromFile(const std::string& filepath) {
    auto json_file = ugdk::filesystem::manager().OpenFile(filepath + ".json");
    if (!json_file)
        throw system::BaseException("File not found: %s.json\n", filepath.c_str());

    auto contents = json_string(json_file->GetContents().c_str());
    if (!libjson::is_valid(contents))
        throw system::BaseException("Invalid json: %s.json\n", filepath.c_str());

    auto frames = libjson::parse(contents)["frames"];
    graphic::GLTexture* gltexture = graphic::GLTexture::CreateFromFile(
            filepath + ".png");

    TextureAtlas* atlas = new TextureAtlas(gltexture, frames.size());
    for (const auto& frame : frames) {
        auto&& frame_info = frame["frame"];
        size_t index = atlas->AddPiece(frame.name().c_str(),
                        math::Integer2D(frame_info["x"].as_int(), frame_info["y"].as_int()),
                        math::Integer2D(frame_info["w"].as_int(), frame_info["h"].as_int()));

        auto& piece = atlas->pieces_[index];
        piece.rotated_90_clockwise = frame["rotated"].as_bool();
        if (frame["trimmed"].as_bool()) {
            auto&& spriteSourceSize = frame["spriteSourceSize"];
            piece.offset.x = spriteSourceSize["x"].as_int();
            piece.offset.y = spriteSourceSize["y"].as_int();
            piece.trimmed_size.x = spriteSourceSize["w"].as_int();
            piece.trimmed_size.y = spriteSourceSize["h"].as_int();
        }
    }
    return atlas;
}

math::Integer2D TextureAtlas::size() const {
    return math::Integer2D(texture_->width(), texture_->height());
}


}  // namespace graphic
}  // namespace ugdk
