#ifndef UGDK_GRAPHIC_DRAWABLE_FUNCTIONS_H_
#define UGDK_GRAPHIC_DRAWABLE_FUNCTIONS_H_

#include <ugdk/graphic.h>
#include <string>

namespace ugdk {
namespace graphic {

float FillBufferWithText(const Font*, const std::u32string&, void* buf, float y = 0.0f);

void DrawSquare(const Geometry& geometry, const VisualEffect&, const Texture*);
void DrawTextLine(const Geometry& geometry, const VisualEffect&, const Font*, const std::string& utf8_message);

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_DRAWABLE_FUNCTIONS_H_
