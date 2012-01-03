#ifndef HORUSEYE_FRAMEWORK_TYPES_H_
#define HORUSEYE_FRAMEWORK_TYPES_H_

#include <SDL/SDL.h>

namespace ugdk {

struct Color {
    Color(float _r = 1.0f, float _g = 1.0f, float _b = 1.0f)
          : r(_r), g(_g), b(_b) {}

    union {
        struct { float r, g, b; };
        struct { float val[3];  };
    };
};

//static Color BLACK = {0.0f, 0.0f, 0.0f};
static const Color WHITE(1.0f, 1.0f, 1.0f);

typedef Uint8 uint8;
typedef Uint16 uint16;
typedef Uint32 uint32;
typedef Uint64 uint64;
typedef Sint8 int8;
typedef Sint16 int16;
typedef Sint32 int32;
typedef Sint64 int64;

typedef Uint8 Mirror;
static const Mirror MIRROR_NONE   = 0;
static const Mirror MIRROR_HFLIP  = 1;
static const Mirror MIRROR_VFLIP  = 2;
static const Mirror MIRROR_HVFLIP = 3;

}  // namespace framework

#endif /* HORUSEYE_FRAMEWORK_TYPES_H_ */
