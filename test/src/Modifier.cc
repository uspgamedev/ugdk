#include "gtest/gtest.h"
#include <ugdk/graphic/modifier.h>

using ugdk::math::Vector2D;
using ugdk::Color;
using ugdk::graphic::Modifier;

#define PI 3.1415926535897932384626433832795

TEST(Modifier, Constructors) {
    {   Modifier m;
    EXPECT_EQ(0.0, m.offset().x);
    EXPECT_EQ(0.0, m.offset().y);
    EXPECT_EQ(0.0, m.rotation());
    EXPECT_EQ(1.0, m.scale().x);
    EXPECT_EQ(1.0, m.scale().y); }

    {   Modifier m(Vector2D(15.0, -10.0), Vector2D(0.5, 0.4), -17.8);
        Modifier copy(m);
        EXPECT_EQ(m.offset().x, copy.offset().x);
        EXPECT_EQ(m.offset().y, copy.offset().y);
        EXPECT_EQ(m.scale().x , copy.scale().x);
        EXPECT_EQ(m.scale().y , copy.scale().y);
        EXPECT_EQ(m.rotation(), copy.rotation());
    }
}

TEST(Modifier, Compose) {
    Modifier id;

    {   Modifier m1(Vector2D(10, 50));
        Modifier comp = id * m1;
        EXPECT_EQ(m1.offset().x, comp.offset().x);
        EXPECT_EQ(m1.offset().y, comp.offset().y);
    }

    {
        Modifier mx(Vector2D(10.0, 0.0));
        Modifier rot;
        rot.set_rotation(PI/4.0);

        Modifier result1 = rot * mx;
        EXPECT_EQ(7.0710678118654755, result1.offset().x);
        EXPECT_EQ(7.0710678118654746, result1.offset().y);

        Modifier result2 = rot * mx * mx;
        EXPECT_EQ(14.142135623730951, result2.offset().x);
        EXPECT_EQ(14.142135623730949, result2.offset().y);
    }

    // TODO: more tests
}