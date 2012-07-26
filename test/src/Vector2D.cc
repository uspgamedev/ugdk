#include "gtest/gtest.h"
#include <ugdk/math/vector2D.h>

using namespace ugdk;

#define PI 3.1415926535897932384626433832795

TEST(Vector2D, Constructor) {
	Vector2D a;
	EXPECT_EQ(0.0, a.x);

	for(int i = 0; i < 10; ++i) {
		Vector2D v(i);
		EXPECT_EQ(v.x, v.y);
	}

	Vector2D c(5.0, 8.0);
	EXPECT_EQ(5.0, c.x);
	EXPECT_EQ(8.0, c.y);
}

TEST(Vector2D, Operators) {
	Vector2D a(5.0, 13.5), b(10.5, -11.0);
	
	{	Vector2D c = a + b;
		EXPECT_EQ(c.x, 15.5);
		EXPECT_EQ(c.y, 2.5); }

	{	Vector2D c = a - b;
		EXPECT_EQ(c.x, -5.5);
		EXPECT_EQ(c.y, 24.5); }

	{	Vector2D c = a * 3.0;
		EXPECT_EQ(c.x, 15.0);
		EXPECT_EQ(c.y, 40.5); }

	{	Vector2D c = a / 2.0;
		EXPECT_EQ(c.x, 2.5);
		EXPECT_EQ(c.y, 6.75); }

	{	double c = a * b;
		EXPECT_EQ(c, -96.0); }
}

TEST(Vector2D, Methods) {
	Vector2D a(5.0, 7.0), b(0.0, -11.0);

	{	EXPECT_EQ(a.Length(), 8.6023252670426267717294735350497); }

	{	EXPECT_EQ(b.Length(), 11.0); }

	{	EXPECT_EQ(a.NormOne(), 12.0); }

	{	EXPECT_EQ(b.NormOne(), 11.0); }

	{	EXPECT_EQ(b.Normalize().y, -1.0); }

	{	EXPECT_EQ(b.Angle(), -PI/2); }

	{	Vector2D f = b.Rotate(PI/2);
		EXPECT_EQ(f.x, 11.0);
		EXPECT_TRUE(f.y < 1.0e-6); }

    {   Vector2D hm = a.HorizontalMirror();
        EXPECT_EQ(hm.x, -5.0);
        EXPECT_EQ(hm.y, 7.0); }

     {   Vector2D vm = a.VerticalMirror();
        EXPECT_EQ(vm.x, 5.0);
        EXPECT_EQ(vm.y, -7.0); }
}