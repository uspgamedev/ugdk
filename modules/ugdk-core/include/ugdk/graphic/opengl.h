#ifndef UGDK_INTERNAL_OPENGL_H_
#define UGDK_INTERNAL_OPENGL_H_

#ifdef UGDK_USING_GLES

#include <GLES2/gl2.h>

#else

#include <GL/glew.h>

#ifndef UGDK_OPENGL_IGNORE_VBO
#define UGDK_OPENGL_USE_VBO
#endif

#endif

#include <cassert>

namespace ugdk {
namespace internal {

inline void AssertNoOpenGLError() {
#ifndef NDEBUG
    auto err = glGetError();
    assert(GL_NO_ERROR == err);
#endif
}

inline void ClearOpenGLErrors() {
#ifndef NDEBUG
    while (GL_NO_ERROR != glGetError());
#endif
}

}
}

#endif // UGDK_INTERNAL_SDLEVENTHANDLER_H_
