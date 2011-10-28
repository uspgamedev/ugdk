#include "gtest/gtest.h"
#include <ugdk/util/animationprotocol.h>

using namespace ugdk;

#define PI 3.1415926535897932384626433832795f

TEST(Vector2D, Constructor) {
	Vector2D a;
	EXPECT_EQ(0.0f, a.x);

	for(int i = 0; i < 10; ++i) {
		Vector2D v(i);
		EXPECT_EQ(v.x, v.y);
	}

	Vector2D c(5.0f, 8.0f);
	EXPECT_EQ(5.0f, c.x);
	EXPECT_EQ(8.0f, c.y);
}

TEST(Vector2D, Operators) {
	Vector2D a(5.0f, 13.5f), b(10.5f, -11.0f);
	
	{	Vector2D c = a + b;
		EXPECT_EQ(c.x, 15.5f);
		EXPECT_EQ(c.y, 2.5f); }

	{	Vector2D c = a - b;
		EXPECT_EQ(c.x, -5.5f);
		EXPECT_EQ(c.y, 24.5f); }

	{	Vector2D c = a * 3.0f;
		EXPECT_EQ(c.x, 15.0f);
		EXPECT_EQ(c.y, 40.5f); }

	{	Vector2D c = a / 2.0f;
		EXPECT_EQ(c.x, 2.5f);
		EXPECT_EQ(c.y, 6.75f); }

	{	float c = a * b;
		EXPECT_EQ(c, -96.0f); }
}

TEST(Vector2D, Methods) {
	Vector2D a(5.0f, 7.0f), b(0.0f, -11.0f);

	{	EXPECT_EQ(a.Length(), 8.6023252670426267717294735350497f); }

	{	EXPECT_EQ(b.Length(), 11.0f); }

	{	EXPECT_EQ(a.NormOne(), 12.0f); }

	{	EXPECT_EQ(b.NormOne(), 11.0f); }

	{	EXPECT_EQ(b.Normalize().y, -1.0f); }

	{	EXPECT_EQ(b.Angle(), -PI/2); }

	{	Vector2D f = b.Rotate(PI/2);
		EXPECT_EQ(f.x, 11.0f);
		EXPECT_TRUE(f.y < 1.0e-6); }
}