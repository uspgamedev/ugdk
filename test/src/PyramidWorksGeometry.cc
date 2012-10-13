#include "gtest/gtest.h"
#include <ugdk/math/vector2D.h>
#include <pyramidworks/geometry/geometricshape.h>
#include <pyramidworks/geometry/rect.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/convexpolygon.h>
#include <vector>

using ugdk::math::Vector2D;
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

	vector<Vector2D> lVertices;
	lVertices.push_back( Vector2D(-1,-1) );
	lVertices.push_back( Vector2D(1,1) );
	ConvexPolygon l(lVertices);

	Circle r (0.5);
	
	{	Vector2D rPos (3,10.25);
		EXPECT_TRUE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (8,6);
		EXPECT_FALSE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (1,7);
		EXPECT_FALSE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (4,4);
		EXPECT_TRUE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (3,3);
		EXPECT_TRUE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (3,2);
		EXPECT_TRUE( cp.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (3.8, 1);
		EXPECT_FALSE( cp.Intersects(cpPos, &r, rPos) ); }
	
	{	Vector2D rPos (1.9,4.1);
		EXPECT_TRUE( l.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (3.25,5.25);
		EXPECT_TRUE( l.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (10,10);
		EXPECT_FALSE( l.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (-8,14);
		EXPECT_FALSE( l.Intersects(cpPos, &r, rPos) ); }

	{	Vector2D rPos (12,-6);
		EXPECT_FALSE( l.Intersects(cpPos, &r, rPos) ); }
}

TEST(ConvexPolygon, ConvexPolygonIntersection) {
	vector<Vector2D> cpVertices;
	cpVertices.push_back( Vector2D(-1,0) );
	cpVertices.push_back( Vector2D(1,6) );
	cpVertices.push_back( Vector2D(2,-2) );
	cpVertices.push_back( Vector2D(1,-3) );
	cpVertices.push_back( Vector2D(-0.5,-2) );
	ConvexPolygon cp (cpVertices);
	Vector2D cpPos (2, 4);

	vector<Vector2D> rVertices;
	rVertices.push_back( Vector2D(-0.5,-0.5) );
	rVertices.push_back( Vector2D(-0.5, 0.5) );
	rVertices.push_back( Vector2D( 0.5, 0.5) );
	rVertices.push_back( Vector2D( 0.5,-0.5) );
	ConvexPolygon rect (rVertices);
	Vector2D rPos (2.5, 3.5);

	vector<Vector2D> tVertices;
	tVertices.push_back( Vector2D(-3,0) );
	tVertices.push_back( Vector2D(2,2) );
	tVertices.push_back( Vector2D(2,-2) );
	ConvexPolygon triangle(tVertices);

	vector<Vector2D> l1Vertices;
	l1Vertices.push_back( Vector2D(-1,1) );
	l1Vertices.push_back( Vector2D(1,-1) );
	ConvexPolygon l1(l1Vertices);

	vector<Vector2D> l2Vertices;
	l2Vertices.push_back( Vector2D(-1,-1) );
	l2Vertices.push_back( Vector2D(1,1) );
	ConvexPolygon l2(l2Vertices);
	
	{	EXPECT_TRUE( cp.Intersects(cpPos, &cp, cpPos) ); }

	{	EXPECT_TRUE( cp.Intersects(cpPos, &rect, rPos) ); }

	{	Vector2D tPos (6,10); /*NOT WORKING - vertices (1 of each polygon) are in the same pos*/
		EXPECT_TRUE( cp.Intersects(cpPos, &triangle, tPos) ); }

	{	Vector2D tPos (6,3);
		EXPECT_TRUE( cp.Intersects(cpPos, &triangle, tPos) ); }

	{	Vector2D tPos (0,8);
		EXPECT_TRUE( cp.Intersects(cpPos, &triangle, tPos) ); }

	{	Vector2D tPos (3, 16);
		EXPECT_FALSE( cp.Intersects(cpPos, &triangle, tPos) ); }

	{	Vector2D tPos (6, 0);
		EXPECT_FALSE( cp.Intersects(cpPos, &triangle, tPos) ); }

	{	Vector2D lPos (0,0);
		EXPECT_TRUE( l1.Intersects(lPos, &l2, lPos) ); }

	{	Vector2D lPos (3,3);
		EXPECT_TRUE( cp.Intersects(cpPos, &l1, lPos) ); }

	{	Vector2D lPos (2,1);
		EXPECT_TRUE( cp.Intersects(cpPos, &l2, lPos) ); }

	{	Vector2D lPos (-6,-10);
		EXPECT_FALSE( cp.Intersects(cpPos, &l2, lPos) ); }
}