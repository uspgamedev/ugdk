#ifndef UGDK_GRAPHIC_DEFAULTSHADERS_H_
#define UGDK_GRAPHIC_DEFAULTSHADERS_H_

#include <ugdk/graphic.h>

namespace ugdk {
namespace graphic {

opengl::ShaderProgram* InterfaceShader();
opengl::ShaderProgram* LightSystemShader();

}  // namespace graphic
}  // namespace ugdk

#endif
