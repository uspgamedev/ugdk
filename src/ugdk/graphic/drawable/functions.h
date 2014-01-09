#ifndef UGDK_GRAPHIC_DRAWABLE_FUNCTIONS_H_
#define UGDK_GRAPHIC_DRAWABLE_FUNCTIONS_H_

#include <ugdk/graphic.h>
#include <string>

namespace ugdk {
namespace graphic {

float FillBufferWithText(const Font*, const std::u32string&, void* buf, float y = 0.0f);

void DrawSquare(const Geometry& geometry, const VisualEffect&, const Texture*);
void DrawLine(const Geometry& geometry, const VisualEffect&, const Font*, const std::string& msg);

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_DRAWABLE_FUNCTIONS_H_
