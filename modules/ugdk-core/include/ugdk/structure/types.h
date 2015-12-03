#ifndef UGDK_STRUCTURE_TYPES_H_
#define UGDK_STRUCTURE_TYPES_H_

#include <stdint.h>

#ifdef SWIG
#pragma SWIG nowarn=312
#endif

namespace ugdk {

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8 Mirror;
static const Mirror MIRROR_NONE     = 0;
static const Mirror MIRROR_HFLIP    = 1;
static const Mirror MIRROR_VFLIP    = 2;
static const Mirror MIRROR_HVFLIP   = 3;

namespace enums {
namespace mirroraxis {
enum MirrorAxis {
    HORZ = 0,
    DIAG_UP = 45,
    VERT = 90,
    DIAG_DOWN = 135
};
}
}

}  // namespace ugdk

#endif /* UGDK_STRUCTURE_TYPES_H_ */
