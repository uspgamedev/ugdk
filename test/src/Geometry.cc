#include "gtest/gtest.h"
#include <ugdk/graphic/geometry.h>

using ugdk::math::Vector2D;
using ugdk::Color;
using ugdk::graphic::Geometry;

#define PI 3.1415926535897932384626433832795

TEST(Geometry, Constructors) {
    {   Geometry m;
    EXPECT_EQ(0.0, m.offset().x);
    EXPECT_EQ(0.0, m.offset().y);
    EXPECT_EQ(0.0, m.rotation());
    EXPECT_EQ(1.0, m.scale().x);
    EXPECT_EQ(1.0, m.scale().y); }

    {   Geometry m(Vector2D(15.0, -10.0), Vector2D(0.5, 0.4), -17.8);
        Geometry copy(m);
        EXPECT_EQ(m.offset().x, copy.offset().x);
        EXPECT_EQ(m.offset().y, copy.offset().y);
        EXPECT_EQ(m.scale().x , copy.scale().x);
        EXPECT_EQ(m.scale().y , copy.scale().y);
        EXPECT_EQ(m.rotation(), copy.rotation());
    }
}

TEST(Geometry, Compose) {
    Geometry id;

    {   Geometry m1(Vector2D(10, 50));
        Geometry comp = id * m1;
        EXPECT_EQ(m1.offset().x, comp.offset().x);
        EXPECT_EQ(m1.offset().y, comp.offset().y);
    }

    {
        Geometry mx(Vector2D(10.0, 0.0));
        Geometry rot;
        rot.set_rotation(PI/4.0);

        Geometry result1 = rot * mx;
        EXPECT_EQ(7.0710678118654755, result1.offset().x);
        EXPECT_EQ(7.0710678118654746, result1.offset().y);

        Geometry result2 = rot * mx * mx;
        EXPECT_EQ(14.142135623730951, result2.offset().x);
        EXPECT_EQ(14.142135623730949, result2.offset().y);
    }

    // TODO: more tests
}