#ifndef UGDK_GRAPHIC_DEFAULTSHADERS_H_
#define UGDK_GRAPHIC_DEFAULTSHADERS_H_

#include <ugdk/graphic.h>

namespace ugdk {
namespace graphic {

ShaderProgram* CreateShader(bool light_system, bool color_text_mode);

ShaderProgram* InterfaceShader();
ShaderProgram* LightSystemShader();
ShaderProgram* InterfaceTextShader();
ShaderProgram* LightSystemTextShader();
ShaderProgram* LightShader();

}  // namespace graphic
}  // namespace ugdk

#endif
