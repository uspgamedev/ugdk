#include "gtest/gtest.h"
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/image.h>
#include <ugdk/graphic/drawable/solidrectangle.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>

using namespace ugdk;

TEST(Drawable, Virtual) {
    {   Sprite s(NULL); }
    {   Image i; }
    {   SolidRectangle r(Vector2D(10.0f, 10.0f)); }
    {   TexturedRectangle r(NULL, Vector2D(10.0f, 10.0f)); }
    EXPECT_TRUE(true);
}