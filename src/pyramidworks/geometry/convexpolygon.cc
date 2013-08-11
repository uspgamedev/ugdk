#include "convexpolygon.h"
#include "rect.h"
#include "circle.h"
#include <cmath>
#include <ugdk/structures/intervalkdtree.h>

namespace pyramidworks {
namespace geometry {

using ugdk::math::Vector2D;

ConvexPolygon::ConvexPolygon(const std::vector<ugdk::math::Vector2D>& vertices) : GeometricShape(), vertices_(vertices) {
	calculateSize();
}

bool ConvexPolygon::Intersects (const ugdk::math::Vector2D& this_pos, const ConvexPolygon *polygon, const ugdk::math::Vector2D& otherpos) const {
	return ! checkAxisSeparation(vertices_, this_pos, polygon->vertices_, otherpos);
}

bool ConvexPolygon::Intersects (const ugdk::math::Vector2D& this_pos, const Rect *rect, const ugdk::math::Vector2D& otherpos) const {
	std::vector<ugdk::math::Vector2D> rect_vertices;
	ugdk::math::Vector2D v1 (-rect->width()/2.0, -rect->height()/2.0);
	ugdk::math::Vector2D v2 ( rect->width()/2.0, -rect->height()/2.0);
	ugdk::math::Vector2D v3 (-rect->width()/2.0,  rect->height()/2.0);
	ugdk::math::Vector2D v4 ( rect->width()/2.0,  rect->height()/2.0);
	rect_vertices.push_back(v1);
	rect_vertices.push_back(v2);
	rect_vertices.push_back(v3);
	rect_vertices.push_back(v4);

	return ! checkAxisSeparation(vertices_, this_pos, rect_vertices, otherpos);
}

bool ConvexPolygon::Intersects (const ugdk::math::Vector2D& this_pos, const Circle *circle, const ugdk::math::Vector2D& circ_pos) const {
	double R = circle->radius(), AC_edge_angle, BC_edge_angle;
	size_t p2;
	ugdk::math::Vector2D A, B, edge, edgeNormal, AC;
	bool isCircleInside = true;
	bool isProjInEdge;
	/* For each edge in the convex polygon, check if the center of the circle is "inside" the polygon. 
	   If this is true for all edges, the center of the circle is inside the polygon area and therefore they intersect. */
	for (size_t i = 0; i < vertices_.size(); i++) {
		p2 = i + 1;
		if (p2 >= vertices_.size())	p2 = 0;

		A = (vertices_[i]  + this_pos);
		B = (vertices_[p2] + this_pos);
		edge = B - A;
		edgeNormal.x = -edge.y;
		edgeNormal.y = edge.x;
		AC = circ_pos - A;

		if (insideSameSpace(edgeNormal, AC)) {
			/* This edge says the center point is "outside" the polygon, so the circle cannot be inside the polygon.
			   So check if the circle intersects with the edge by calculating the distance between the center and the edge
			   using projection. */
			isCircleInside = false;
			edgeNormal = edgeNormal.Normalize();
			edge = edge.Normalize();
			AC_edge_angle = AC.Normalize() * edge;
			BC_edge_angle = (circ_pos-B).Normalize() * (edge*-1);
			isProjInEdge = ( 0 <= AC_edge_angle && AC_edge_angle <= 1.0) && ( 0 <= BC_edge_angle && BC_edge_angle <= 1.0 );
			/* If the projection of the center to the edge line is in the edge "space" and the distance is less than the circle radius, it is intersecting.*/
			if		(isProjInEdge && (edgeNormal * AC) <= R)	return true;
			/* Else if either extremity of the edge is close to the center point, we're also intersecting.*/
			else if ( (     AC     ).LengthSquared() <= R*R)	return true;
			else if ( (B - circ_pos).LengthSquared() <= R*R)	return true;
		}
	}
	return isCircleInside;
}

bool ConvexPolygon::Intersects (const ugdk::math::Vector2D& this_pos, const GeometricShape *obj, const ugdk::math::Vector2D& that_pos) const {
    return obj->Intersects(that_pos, this, this_pos);
}

ugdk::ikdtree::Box<2> ConvexPolygon::GetBoundingBox(const ugdk::math::Vector2D& thispos) const {
    ugdk::math::Vector2D thisposmin(thispos.x - bbox_half_width_,
                        thispos.y - bbox_half_height_);
    ugdk::math::Vector2D thisposmax(thispos.x + bbox_half_width_,
                        thispos.y + bbox_half_height_);
    return ugdk::ikdtree::Box<2>(thisposmin.val, thisposmax.val);
}

void ConvexPolygon::calculateSize() {
	/* Calculate rough width and height of the convex polygon in order to build a bounding box. */
	for (size_t i=0; i < vertices_.size(); i++) {
		if (fabs(vertices_[i].x) > bbox_half_width_)
			bbox_half_width_ = fabs(vertices_[i].x);
		if (fabs(vertices_[i].y) > bbox_half_height_)
			bbox_half_height_ = fabs(vertices_[i].y);
	}
}

bool ConvexPolygon::checkAxisSeparation(const std::vector<ugdk::math::Vector2D>& obj1, const ugdk::math::Vector2D& obj1pos,
										const std::vector<ugdk::math::Vector2D>& obj2, const ugdk::math::Vector2D& obj2pos) const {
	size_t i, p2;
	/* For each edge in the obj1 polygon, we check if that edge is a separating axis between the two polygons. */
	for (i = 0; i < obj1.size(); i++) {
		p2 = i + 1;
		if (p2 >= obj1.size())	p2 = 0;

		/* obj1 and obj2 ugdk::math::Vector2Ds are in local coordinates relative to that polygon - so add the polygon position to the
		   obj1 vectors we are passing to the separating axis edge test. */
		if (axisSeparationTest(obj1[i]+obj1pos, obj1[p2]+obj1pos, obj1pos, obj2, obj2pos))
			return true;
	}

	/* In order for this test work properly, we need to check *every*  edge if it is a separating axis, so that includes
	   also testing obj2 edges. */
	for (i = 0; i < obj2.size(); i++) {
		p2 = i + 1;
		if (p2 >= obj2.size())	p2 = 0;

		if (axisSeparationTest(obj2[i]+obj2pos, obj2[p2]+obj2pos, obj2pos, obj1, obj1pos))
			return true;
	}

	return false;
}

bool ConvexPolygon::axisSeparationTest(const ugdk::math::Vector2D& p1, const ugdk::math::Vector2D& p2, const ugdk::math::Vector2D& ref,
									   const std::vector<ugdk::math::Vector2D>& obj, const ugdk::math::Vector2D& obj2pos) const {
	ugdk::math::Vector2D edge = p2 - p1;
	ugdk::math::Vector2D edgeNormal (-edge.y, edge.x);

	/* Store the side of the reference point in the given polygon (obj1) relative to the given edge (p2-p1) to check against. */
	bool ref_side = insideSameSpace(edgeNormal, (ref - p1));

	/* Now check in which side of the edge each vertex of the obj2 polygon is. */
	for (size_t i = 0; i<obj.size(); i++) {
		bool side = insideSameSpace(edgeNormal, ((obj[i]+obj2pos) - p1));
		if (side == ref_side) {
			/* The point we're checking (of the other polygon) is in the same side as our reference point (from our polygon),
				so this edge cannot be a separating axis. */
			return false;
		}
	}

	/* All vertices of the obj2 polygon are on the opposing side of the given reference point relative to the edge, so this
	   edge is a separating axis. */
	return true;
}

bool ConvexPolygon::insideSameSpace(const ugdk::math::Vector2D& line, const ugdk::math::Vector2D& point) const {
	return (line * point) > 0;
}

} // namespace geometry
} // namespace pyramidworks
