#include "gtest/gtest.h"
#include <ugdk/graphic/modifier.h>

using ugdk::Vector2D;
using ugdk::Color;
using ugdk::graphic::Modifier;

#define PI 3.1415926535897932384626433832795

TEST(Modifier, Constructors) {
    {   Modifier m;
    EXPECT_EQ(0.0, m.offset().x);
    EXPECT_EQ(0.0, m.offset().y);
    EXPECT_EQ(ugdk::MIRROR_NONE, m.mirror());
    EXPECT_EQ(0.0, m.rotation());
    EXPECT_TRUE(m.visible());
    EXPECT_EQ(1.0, m.scale().x);
    EXPECT_EQ(1.0, m.scale().y); }

    {   Modifier m(Vector2D(15.0, -10.0), Vector2D(0.5, 0.4), -17.8, ugdk::MIRROR_HFLIP, Color(0xFF5020), false);
        Modifier copy(m);
        EXPECT_EQ(m.offset().x, copy.offset().x);
        EXPECT_EQ(m.offset().y, copy.offset().y);
        EXPECT_EQ(m.scale().x , copy.scale().x);
        EXPECT_EQ(m.scale().y , copy.scale().y);
        EXPECT_EQ(m.rotation(), copy.rotation());
        EXPECT_EQ(m.mirror()  , copy.mirror());
        EXPECT_EQ(m.color().r , copy.color().r);
        EXPECT_EQ(m.color().g , copy.color().g);
        EXPECT_EQ(m.color().b , copy.color().b);
        EXPECT_EQ(m.color().a , copy.color().a);
        EXPECT_EQ(m.flags()   , copy.flags());
        EXPECT_EQ(m.visible() , copy.visible());
    }
}

TEST(Modifier, Compose) {
    Modifier id;

    {   Modifier m1(Vector2D(10, 50));
        Modifier* compP = Modifier::Compose(&id, &m1);
        Modifier comp = *compP;
        delete compP;
        EXPECT_EQ(m1.offset().x, comp.offset().x);
        EXPECT_EQ(m1.offset().y, comp.offset().y);
    }

    // TODO: more tests
}