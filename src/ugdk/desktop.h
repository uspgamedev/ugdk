#ifndef UGDK_DESKTOP_H_
#define UGDK_DESKTOP_H_

#include <ugdk/system/config.h>

namespace ugdk {
namespace desktop {

class Manager;
class Window;
struct WindowSettings;

#ifdef UGDK_3D_ENABLED
namespace threed {
class Manager;
}
#endif

}  // namespace desktop
}  // namespace ugdk

#endif // UGDK_DESKTOP_H_
