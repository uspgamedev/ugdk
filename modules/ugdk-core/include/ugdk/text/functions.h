#ifndef UGDK_TEXT_FUNCTIONS_H_
#define UGDK_TEXT_FUNCTIONS_H_

#include <ugdk/text.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/vertexdata.h>
#include <string>

namespace ugdk {
namespace text {

float FillBufferWithText(const Font*, const std::u32string&, graphic::VertexData& vtx_data, float y = 0.0f);

void DrawTextLine(graphic::Canvas& canvas, const Font*, const std::string& utf8_message);

}  // namespace text
}  // namespace ugdk

#endif // UGDK_TEXT_FUNCTIONS_H_
