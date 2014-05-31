#include <ugdk/graphic/textureatlas.h>

#include <ugdk/internal/gltexture.h>
#include <ugdk/system/engine.h>
#include <libjson.h>

namespace ugdk {
namespace graphic {

void TextureAtlas::BoundPiece::ConvertToAtlas(float *u, float *v) const {
    ConvertToAtlas(*u, *v, u, v);
}
        
void TextureAtlas::BoundPiece::ConvertToAtlas(float in_u, float in_v, float *out_u, float *out_v) const {
    *out_u = (piece_->position.x + (in_u) * piece_->size.x) / atlas_->texture_->width();
    *out_v = (piece_->position.y + (in_v) * piece_->size.y) / atlas_->texture_->height();
}

TextureAtlas::TextureAtlas(const internal::GLTexture* texture, std::size_t size)
:   texture_(texture)
{
    pieces_.reserve(size);
}

TextureAtlas::~TextureAtlas() {}
    
TextureAtlas* TextureAtlas::LoadFromFile(const std::string& filepath) {
    auto&& contents = system::GetFileContents(filepath + ".json");
    if (!libjson::is_valid(contents))
        throw love::Exception("Invalid json: %s.json\n", filepath.c_str());

    auto frames = libjson::parse(contents)["frames"];
    internal::GLTexture* gltexture = internal::GLTexture::CreateFromFile(filepath + ".png");

    TextureAtlas* atlas = new TextureAtlas(gltexture, frames.size());
    for (const auto& frame : frames) {
        auto&& frame_info = frame["frame"];
        // Ignoring frame["trimmed"]
        // Ignoring frame["rotated"]
        atlas->AddPiece(frame.name(),
                        math::Integer2D(frame_info["x"].as_int(), frame_info["y"].as_int()),
                        math::Integer2D(frame_info["w"].as_int(), frame_info["h"].as_int()));
    }
    return atlas;
}

math::Integer2D TextureAtlas::size() const {
    return math::Integer2D(texture_->width(), texture_->height());
}
    

}  // namespace graphic
}  // namespace ugdk
