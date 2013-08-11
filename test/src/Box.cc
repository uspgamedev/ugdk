#include "gtest/gtest.h"
#include <ugdk/structure/box.h>
#include <ugdk/math/vector2D.h>
#include <array>

TEST(Box, CopyConstruct) {
    using namespace ugdk::structure;
    using ugdk::math::Vector2D;

    Box<2> b1(Vector2D(0.5, 1.5), Vector2D(1.0, 2.0));
    Box<2> c(b1);
    EXPECT_EQ(b1.min_coordinates(), c.min_coordinates());
    EXPECT_EQ(b1.max_coordinates(), c.max_coordinates());
}

TEST(Box, Update) {
    using namespace ugdk::structure;
    using ugdk::math::Vector2D;

    Box<2> b1(Vector2D(0.5, 1.5), Vector2D(1.0, 2.0));
    Box<2> b2(Vector2D(0.0, 1.5), Vector2D(0.5, 2.0));

    EXPECT_NE(b1.min_coordinates(), b2.min_coordinates());
    EXPECT_NE(b1.max_coordinates(), b2.max_coordinates());

    b1.UpdateBox(b2);
    EXPECT_EQ(b2.min_coordinates(), b1.min_coordinates());
    EXPECT_EQ(b2.max_coordinates(), b1.max_coordinates());
}

