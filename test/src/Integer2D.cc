#include "gtest/gtest.h"
#include <ugdk/math/integer2D.h>

using ugdk::math::Integer2D;

TEST(Integer2D, Constructor) {
	Integer2D a;
	EXPECT_EQ(0, a.x);

	for(int i = 0; i < 10; ++i) {
		Integer2D v(i);
		EXPECT_EQ(v.x, v.y);
	}

	Integer2D c(5, 8);
	EXPECT_EQ(5, c.x);
	EXPECT_EQ(8, c.y);
}

TEST(Integer2D, Operators) {
	Integer2D a(5, 13), b(10, -11);
	
	{	Integer2D c = a + b;
		EXPECT_EQ(c.x, 15);
		EXPECT_EQ(c.y, 2); }

	{	Integer2D c = a - b;
		EXPECT_EQ(c.x, -5);
		EXPECT_EQ(c.y, 24); }

	{	Integer2D c = a * 3;
		EXPECT_EQ(c.x, 15);
		EXPECT_EQ(c.y, 39); }

	{	Integer2D c = a / 2;
		EXPECT_EQ(c.x, 2);
		EXPECT_EQ(c.y, 6); }

	{	int c = a * b;
		EXPECT_EQ(c, -93); }
		
	{  Integer2D c = b % a;
		EXPECT_EQ(0,c.x);
		EXPECT_EQ(-11,c.y); }
}

TEST(Integer2D, Methods) {
	Integer2D a(5, 7), b(0, -11), c(3,2);

	{	EXPECT_EQ(8.6023252670426267717294735350497, a.Length()); }

	{	EXPECT_EQ(11.0, b.Length()); }

	{	EXPECT_EQ(12, a.NormOne()); }

	{	EXPECT_EQ(11, b.NormOne()); }


	{	EXPECT_EQ(-PI/2, b.Angle()); }

    {	Integer2D f = b.Rotated(ugdk::math::NINETY);
		EXPECT_EQ(11, f.x);
		EXPECT_EQ(0, f.y); }

    {   Integer2D hm = a.Mirrored(mirroraxis::VERT);
        EXPECT_EQ(-5, hm.x);
        EXPECT_EQ(7, hm.y); }

    {  Integer2D vm = a.Mirrored(mirroraxis::HORZ);
        EXPECT_EQ(5, vm.x);
        EXPECT_EQ(-7, vm.y); }
		
    {  Integer2D t = a.Mirrored(mirroraxis::DIAG_UP);
        EXPECT_EQ(7, t.x);
        EXPECT_EQ(5, t.y); }
		
    {  Integer2D t = b.Mirrored(mirroraxis::DIAG_DOWN);
        EXPECT_EQ(11, t.x);
        EXPECT_EQ(0, t.y); }
		
	{ Integer2D t = a.Scaled(b,c);
		EXPECT_EQ(0,t.x);
		EXPECT_EQ(-38,t.y); }
		
	{ Integer2D t = b.Remainder(a);
		EXPECT_EQ(0,t.x);
		EXPECT_EQ(-4,t.y); }
}
