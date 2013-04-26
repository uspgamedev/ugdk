#include <ugdk/graphic/drawable/multitext.h>

#include <ugdk/graphic/drawable/text.h>
#include <ugdk/graphic/geometry.h>

namespace ugdk {
namespace graphic {

using std::vector;
using std::wstring;

MultiText::MultiText(const vector<wstring>& message, freetypeglxx::TextureFont* font)
    : font_(font) {
        SetMessage(message);
}

MultiText::~MultiText() {
    for(vector<Text*>::iterator it = texts_.begin(); it != texts_.end(); ++it)
        delete *it;
    texts_.clear();
}

void MultiText::SetMessage(const vector<wstring>& message) {
    for(vector<Text*>::iterator it = texts_.begin(); it != texts_.end(); ++it)
        delete *it;
    texts_.clear();

    size_ = math::Vector2D();
    for(vector<wstring>::const_iterator it = message.begin(); it != message.end(); ++it) {
        texts_.push_back(new Text(*it, font_));
        size_.x = std::max(size_.x, texts_.back()->size().x);
        size_.y += texts_.back()->size().y;
    }
}

void MultiText::Draw(const Geometry& geo, const VisualEffect& eff) const {
    double off_y = 0.0;
    for(vector<Text*>::const_iterator it = texts_.begin(); it != texts_.end(); ++it) {
        (*it)->Draw(geo * Geometry(math::Vector2D(0.0, off_y) - hotspot_), eff);
        off_y += (*it)->size().y;
    }
}

}  // namespace graphic
}  // namespace ugdk
