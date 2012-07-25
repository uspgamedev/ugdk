#include "gtest/gtest.h"
#include <ugdk/math/vector2D.h>
#include <pyramidworks/geometry/geometricshape.h>
#include <pyramidworks/geometry/rect.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/convexpolygon.h>
#include <vector>

using ugdk::Vector2D;
using std::vector;
using namespace pyramidworks::geometry;

TEST(ConvexPolygon, BoundingBox) {
	vector<Vector2D> vertices;
	vertices.push_back( Vector2D(-1,0) );
	vertices.push_back( Vector2D(0,1) );
	vertices.push_back( Vector2D(1,0) );
	vertices.push_back( Vector2D(0,-1) );
	ConvexPolygon cp (vertices);

	EXPECT_EQ(2.0, cp.bbox_width());
	EXPECT_EQ(2.0, cp.bbox_height());
}

TEST(ConvexPolygon, RectIntersection) {
	vector<Vector2D> vertices;
	vertices.push_back( Vector2D(-1,0) );
	vertices.push_back( Vector2D(1,6) );
	vertices.push_back( Vector2D(2,-2) );
	vertices.push_back( Vector2D(1,-3) );
	vertices.push_back( Vector2D(-0.5,-2) );
	ConvexPolygon cp (vertices);
	Vector2D cpPos (2, 4);

	Rect r (2.8, 0.9);
	
	{	Vector2D rPos (5,2);
		EXPECT_TRUE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (5,3);
		EXPECT_TRUE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (5,1);
		EXPECT_FALSE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (3,9);
		EXPECT_TRUE( cp.Intersects(cpPos, &r, rPos) ); }
}

TEST(ConvexPolygon, CircleIntersection) {
	vector<Vector2D> vertices;
	vertices.push_back( Vector2D(-1,0) );
	vertices.push_back( Vector2D(1,6) );
	vertices.push_back( Vector2D(2,-2) );
	vertices.push_back( Vector2D(1,-3) );
	vertices.push_back( Vector2D(-0.5,-2) );
	ConvexPolygon cp (vertices);
	Vector2D cpPos (2, 4);

	Circle r (0.5);
	
	{	Vector2D rPos (3,10.25);
		EXPECT_TRUE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (8,6);
		EXPECT_FALSE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (1,7);
		EXPECT_FALSE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (4,4);   /**/
		EXPECT_TRUE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (3,3);
		EXPECT_TRUE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (3,2);
		EXPECT_TRUE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (3.8, 1);
		EXPECT_FALSE( cp.Intersects(cpPos, &r, rPos) ); }
}