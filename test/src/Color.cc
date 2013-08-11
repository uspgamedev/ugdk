#include "gtest/gtest.h"
#include <ugdk/structure/types.h>

namespace {

using ugdk::Color;

TEST(Color, Constructors) {
    Color id;
    EXPECT_EQ(1.0, id.r);
    EXPECT_EQ(1.0, id.g);
    EXPECT_EQ(1.0, id.b);
    EXPECT_EQ(1.0, id.a);

    Color rgb(0.1, 0.7, 0.3);
    EXPECT_EQ(0.1, rgb.r);
    EXPECT_EQ(0.7, rgb.g);
    EXPECT_EQ(0.3, rgb.b);
    EXPECT_EQ(1.0, rgb.a);

    Color hex(0xFF0020, 0.3);
    EXPECT_EQ(1.0, hex.r);
    EXPECT_EQ(0.0, hex.g);
    EXPECT_EQ(0.12549019607843137, hex.b);
    EXPECT_EQ(0.3, hex.a);
}

static void compareColors(const Color& expect, const Color& actual) {
    EXPECT_EQ(expect.r, actual.r);
    EXPECT_EQ(expect.g, actual.g);
    EXPECT_EQ(expect.b, actual.b);
    EXPECT_EQ(expect.a, actual.a);
}

TEST(Color, Compose) {
    Color id;
    Color color1(0.2, 1.0, 1.0);
    Color color2(0.8, 0.4, 1.0);
    Color color3(1.0, 0.5, 1.0, 0.75);

    Color id_color1 = id * color1;
    Color id_color2 = id;
    id_color2 *= color2;

    compareColors(color1, id_color1);
    compareColors(color2, id_color2);

    Color mix1 = color2 * color3;
    Color mix2 = color1;
    mix2 *= color3;

    compareColors(Color(0.8, 0.2, 1.0, 0.75), mix1);
    compareColors(Color(0.2, 0.5, 1.0, 0.75), mix2);
}

}