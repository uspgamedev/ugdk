#include <ugdk/base/texturecontainer.h>
#include <ugdk/graphic/texture.h>

namespace ugdk {
namespace base {

graphic::Texture* TextureContainer::Load(const std::string& filepath, const std::string& tag) {
    graphic::Texture* tex = this->Find(tag);
    if(tex != NULL) return tex;
    
    tex = graphic::Texture::CreateFromFile(filepath);
    this->Replace(tag, tex);
    return tex;
}

} // namespace base
} // namespace ugdk
