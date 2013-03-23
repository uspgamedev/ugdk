#include "gtest/gtest.h"
#include <glm/glm.hpp>
#include <ugdk/graphic/geometry.h>

using ugdk::math::Vector2D;
using ugdk::Color;
using ugdk::graphic::Geometry;

#define PI 3.1415926535897932384626433832795

TEST(Geometry, Constructors) {
    Geometry m;
    EXPECT_EQ(glm::mat4(), m.AsMat4());

    {   Geometry m(Vector2D(15.0, -10.0), Vector2D(0.5, 0.4), -17.8);
        Geometry copy(m);
        EXPECT_EQ(m.AsMat4(), copy.AsMat4());
    }
}

TEST(Geometry, Compose) {
    Geometry id;

    {   Geometry m1(Vector2D(10, 50));
        Geometry comp = id * m1;
        EXPECT_EQ(m1.AsMat4(), comp.AsMat4());
    }

    {
        Geometry mx(Vector2D(10.0, 0.0));
        Geometry rot(Vector2D(), Vector2D(1.0, 1.0), PI/4.0);

        Geometry result1 = rot * mx;
        glm::mat4 result1_mat = result1.AsMat4();
        EXPECT_EQ(7.0710678118654755f, result1_mat[3].x);
        EXPECT_EQ(7.0710678118654746f, result1_mat[3].y);

        Geometry result2 = rot * mx * mx;
        glm::mat4 result2_mat = result2.AsMat4();
        EXPECT_EQ(14.142135623730951f, result2_mat[3].x);
        EXPECT_EQ(14.142135623730949f, result2_mat[3].y);
    }
    // TODO: more tests
}