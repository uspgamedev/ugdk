#include "convexpolygon.h"
#include "rect.h"
#include "circle.h"
#include <cmath>
#include <ugdk/util/intervalkdtree.h>

#define SQRT_TWO 1.4142136

namespace pyramidworks {
namespace geometry {

using ugdk::Vector2D;

ConvexPolygon::ConvexPolygon(const std::vector<ugdk::Vector2D>& vertices) : GeometricShape(), vertices_(vertices) {
	calculateSize();
}

bool ConvexPolygon::Intersects (const ugdk::Vector2D& this_pos, const ConvexPolygon *polygon, const ugdk::Vector2D& otherpos) const {
	return ! checkAxisSeparation(vertices_, this_pos, polygon->vertices_, otherpos);
}

bool ConvexPolygon::Intersects (const ugdk::Vector2D& this_pos, const Rect *rect, const ugdk::Vector2D& otherpos) const {
	std::vector<Vector2D> rect_vertices;
	Vector2D v1 (-rect->width()/2.0, -rect->height()/2.0);
	Vector2D v2 ( rect->width()/2.0, -rect->height()/2.0);
	Vector2D v3 (-rect->width()/2.0,  rect->height()/2.0);
	Vector2D v4 ( rect->width()/2.0,  rect->height()/2.0);
	rect_vertices.push_back(v1);
	rect_vertices.push_back(v2);
	rect_vertices.push_back(v3);
	rect_vertices.push_back(v4);

	return ! checkAxisSeparation(vertices_, this_pos, rect_vertices, otherpos);
}

bool ConvexPolygon::Intersects (const ugdk::Vector2D& this_pos, const Circle *circle, const ugdk::Vector2D& circ_pos) const {
	/*
	circulo = centro C, raio R
	pra cada aresta E (A-B) do convexo:
		bool io = pontoEstaDentro(E, C)
		if !io:
			D =  (AB/|AB |) * <AB, AC>
			if |D-C|² <= R²:   RETURN COLIDE
			else if |A-C|² <= R²:  RETURN COLIDE
			else if |B-C|² <= R²:  RETURN COLIDE
			RETURN NAO-COLIDE
	RETURN COLIDE-CIRCULO-DENTRO
	*/
	double R = circle->radius();
	int p2;
	Vector2D A, B, edge, edgeNormal, proj, AC;
	bool isCircleInside = true;
	for (int i = 0; i < vertices_.size(); i++) {
		p2 = i + 1;
		if (p2 >= vertices_.size())	p2 = 0;

		A = (vertices_[i]  + this_pos);
		B = (vertices_[p2] + this_pos);
		edge = B - A;
		edgeNormal.x = -edge.y;
		edgeNormal.y = edge.x;

		if (!insideSameSpace(edgeNormal, circ_pos)) {
			isCircleInside = false;
			AC = circ_pos - A;
			proj = edge.Normalize() * (edge * AC);
			if		((proj-circ_pos).LengthSquared() <= R*R)	return true;
			else if ( (A - circ_pos).LengthSquared() <= R*R)	return true;
			else if ( (B - circ_pos).LengthSquared() <= R*R)	return true;
		}
	}
	return isCircleInside;
}

bool ConvexPolygon::Intersects (const ugdk::Vector2D& this_pos, const GeometricShape *obj, const ugdk::Vector2D& that_pos) const {
    return obj->Intersects(that_pos, this, this_pos);
}

ugdk::ikdtree::Box<2> ConvexPolygon::GetBoundingBox(const ugdk::Vector2D& thispos) const {
    Vector2D thisposmin(thispos.x - bbox_half_width_,
                        thispos.y - bbox_half_height_);
    Vector2D thisposmax(thispos.x + bbox_half_width_,
                        thispos.y + bbox_half_height_);
    return ugdk::ikdtree::Box<2>(thisposmin.val, thisposmax.val);
}

void ConvexPolygon::calculateSize() {
	for (int i=0; i < vertices_.size(); i++) {
		if (fabs(vertices_[i].x) > bbox_half_width_)
			bbox_half_width_ = fabs(vertices_[i].x);
		if (fabs(vertices_[i].y) > bbox_half_height_)
			bbox_half_height_ = fabs(vertices_[i].y);
	}
}

bool ConvexPolygon::checkAxisSeparation(const std::vector<ugdk::Vector2D>& obj1, const ugdk::Vector2D& obj1pos,
										const std::vector<ugdk::Vector2D>& obj2, const ugdk::Vector2D& obj2pos) const {
	int p2, ref;
	for (int i = 0; i < obj1.size(); i++) {
		p2 = i + 1;
		if (p2 >= obj1.size())	p2 = 0;

		ref = 0;
		if (i == 0) {
			if (obj1.size() == 2)	ref = 0;
			else if (obj1.size() == 3)	ref = 2;
			else	ref = 0;
		}

		if (axisSeparationTest(obj1[i]+obj1pos, obj1[p2]+obj1pos, obj1pos, obj2, obj2pos))
			return true;
	}
	return false;
}

bool ConvexPolygon::axisSeparationTest(const ugdk::Vector2D& p1, const ugdk::Vector2D& p2, const ugdk::Vector2D& ref,
									   const std::vector<ugdk::Vector2D>& obj, const ugdk::Vector2D& obj2pos) const {
	Vector2D edge = p2 - p1;
	Vector2D edgeNormal (-edge.y, edge.x);

	bool ref_side = insideSameSpace(edgeNormal, (ref - p1));

	for (int i = 0; i<obj.size(); i++) {
		bool side = insideSameSpace(edgeNormal, ((obj[i]+obj2pos) - p1));
		if (side == ref_side) {
			/* The point we're checking (of the other polygon) is in the same side as our reference point (from our polygon),
				so this edge cannot be a separating axis. */
			return false;
		}
	}
	return true;
}

bool ConvexPolygon::insideSameSpace(const ugdk::Vector2D& line, const ugdk::Vector2D& point) const {
	return (line * point) >= 0;
}

} // namespace geometry
} // namespace pyramidworks
