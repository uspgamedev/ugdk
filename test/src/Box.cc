#include "gtest/gtest.h"
#include <ugdk/structure/box.h>
#include <array>

std::array<double, 2> makeArray(double a, double b) {
    std::array<double, 2> result;
    result[0] = a;
    result[1] = b;
    return result;
}

TEST(Box, Copy) {
    using namespace ugdk::structure;

    Box<2> b1(makeArray(0.5, 1.5), makeArray(1.0, 2.0));
    Box<2> b2(makeArray(0.0, 1.5), makeArray(0.5, 2.0));

    Box<2> c(b1);
    EXPECT_EQ(b1.min_coordinates(), c.min_coordinates());
    EXPECT_EQ(b1.max_coordinates(), c.max_coordinates());

    b1 = b2;
    EXPECT_EQ(b2.min_coordinates(), b1.min_coordinates());
    EXPECT_EQ(b2.max_coordinates(), b1.max_coordinates());
}

