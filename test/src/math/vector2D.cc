#include <math.h>
#include <ugdk/math/vector2D.h>
#include "gtest/gtest.h"

using namespace ugdk;

TEST(Vector2D, Base) {
	Vector2D v(5.0f, 8.0f);
	EXPECT_EQ(5.0f, v.x);
	EXPECT_EQ(8.0f, v.y);
}
