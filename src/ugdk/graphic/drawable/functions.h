#ifndef UGDK_GRAPHIC_DRAWABLE_FUNCTIONS_H_
#define UGDK_GRAPHIC_DRAWABLE_FUNCTIONS_H_

#include <ugdk/internal.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/vertexdata.h>
#include <string>

namespace ugdk {
namespace graphic {

float FillBufferWithText(const Font*, const std::u32string&, VertexData::Mapper& mapped_data, float y = 0.0f);

void DrawSquare(const Geometry& geometry, const VisualEffect&, const internal::GLTexture*);
void DrawTextLine(const Geometry& geometry, const VisualEffect&, const Font*, const std::string& utf8_message);

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_DRAWABLE_FUNCTIONS_H_
