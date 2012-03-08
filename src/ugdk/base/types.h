#ifndef HORUSEYE_FRAMEWORK_TYPES_H_
#define HORUSEYE_FRAMEWORK_TYPES_H_

#include <stdint.h>

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
static const Mirror MIRROR_NONE   = 0;
static const Mirror MIRROR_HFLIP  = 1;
static const Mirror MIRROR_VFLIP  = 2;
static const Mirror MIRROR_HVFLIP = 3;

typedef struct Color {
    explicit Color(double _r = 1.0, double _g = 1.0, double _b = 1.0, double _a = 1.0)
          : r(_r), g(_g), b(_b), a(_a) {}
    explicit Color(uint32 hex_val, double _a = 1.0) :
        r(((hex_val & 0xFF0000) >> 16) / 255.0),
        g(((hex_val & 0x00FF00) >>  8) / 255.0),
        b(((hex_val & 0x0000FF)      ) / 255.0),
        a(_a) {}
    
	union {
        struct { double r, g, b, a; };
        struct { double val[4];  };
    };
} Color;

//static Color BLACK = {0.0, 0.0, 0.0};
static const Color WHITE(1.0, 1.0, 1.0);

}  // namespace ugdk

#endif /* HORUSEYE_FRAMEWORK_TYPES_H_ */
