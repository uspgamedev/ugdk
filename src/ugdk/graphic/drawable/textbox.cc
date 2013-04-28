#include <ugdk/graphic/drawable/textbox.h>

#include <ugdk/graphic/drawable/label.h>
#include <ugdk/graphic/geometry.h>

namespace ugdk {
namespace graphic {

using std::vector;
using std::wstring;

TextBox::TextBox(const vector<wstring>& message, Font* font)
    : font_(font) {
        SetMessage(message);
}

TextBox::~TextBox() {
    for(vector<Label*>::iterator it = labels_.begin(); it != labels_.end(); ++it)
        delete *it;
    labels_.clear();
}

void TextBox::SetMessage(const vector<wstring>& message) {
    for(vector<Label*>::iterator it = labels_.begin(); it != labels_.end(); ++it)
        delete *it;
    labels_.clear();

    size_ = math::Vector2D();
    for(vector<wstring>::const_iterator it = message.begin(); it != message.end(); ++it) {
        labels_.push_back(new Label(*it, font_));
        size_.x = std::max(size_.x, labels_.back()->size().x);
        size_.y += labels_.back()->size().y;
    }
}

void TextBox::Draw(const Geometry& geo, const VisualEffect& eff) const {
    double off_y = 0.0;
    for(vector<Label*>::const_iterator it = labels_.begin(); it != labels_.end(); ++it) {
        (*it)->Draw(geo * Geometry(math::Vector2D(0.0, off_y) - hotspot_), eff);
        off_y += (*it)->size().y;
    }
}

}  // namespace graphic
}  // namespace ugdk
