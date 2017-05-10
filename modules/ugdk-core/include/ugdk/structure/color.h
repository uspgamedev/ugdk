#ifndef UGDK_STRUCTURE_COLOR_H_
#define UGDK_STRUCTURE_COLOR_H_

#include <ugdk/structure/types.h>

namespace ugdk {
namespace structure {

struct Color {
    Color() : r(1.0), g(1.0), b(1.0), a(1.0) {}
    explicit Color(double _r, double _g, double _b, double _a = 1.0)
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

    void Compose(const Color& rhs) {
        r *= rhs.r;
        g *= rhs.g;
        b *= rhs.b;
        a *= rhs.a;
    }

    Color&  operator*=(const Color& rhs) { Compose(rhs); return *this; }
    Color   operator*(const Color& rhs) const {
        Color result(rhs);
        result *= *this;
        return result;
    }
    
    double get_r() const { return r; }
    double get_g() const { return g; }
    double get_b() const { return b; }
    double get_a() const { return a; }
    
    void set_r(double r_)  { r = r_; }
    void set_g(double g_)  { g = g_; }
    void set_b(double b_)  { b = b_; }
    void set_a(double a_)  { a = a_; }
    
};

//static Color BLACK = {0.0, 0.0, 0.0};
static const Color WHITE(1.0, 1.0, 1.0);

} // namespace structure
} // namespace ugdk

#endif /* UGDK_STRUCTURE_COLOR_H_ */
