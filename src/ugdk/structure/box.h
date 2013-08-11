#ifndef UGDK_UTIL_BOX_H_
#define UGDK_UTIL_BOX_H_

#include <array>
#include <ugdk/structure.h>

namespace ugdk {
namespace structure {

template <int DIMENSIONS>
class Box {
  public:
    typedef std::array<Coordinate, DIMENSIONS> Point;

    Box();
    Box(const Point& min_coordinates, const Point& max_coordinates);
    virtual ~Box();

    void UpdateBox(const Box&);
    bool IsBelow(int depth, Coordinate boundary) const;
    bool IsAbove(int depth, Coordinate boundary) const;
    bool Contains(const Box& box) const;
    bool Intersects(const Box *box) const;

    const Point& min_coordinates() const;
    const Point& max_coordinates() const;

  protected:
    Point min_coordinates_;
    Point max_coordinates_;
};

//************* Implementation **************

template <int DIMENSIONS>
Box<DIMENSIONS>::Box() {}

template <int DIMENSIONS>
Box<DIMENSIONS>::Box(const Point& min_coordinates, const Point& max_coordinates)
     : min_coordinates_(min_coordinates), max_coordinates_(max_coordinates) {}

template <int DIMENSIONS>
Box<DIMENSIONS>::~Box () {}

template <int DIMENSIONS>
void Box<DIMENSIONS>::UpdateBox(const Box& box) {
    *this = box;
}

template <int DIMENSIONS>
bool Box<DIMENSIONS>::IsBelow (int depth, Coordinate boundary) const {
    return max_coordinates_[depth % DIMENSIONS] <= boundary;
}

template <int DIMENSIONS>
bool Box<DIMENSIONS>::IsAbove (int depth, Coordinate boundary) const {
    return boundary < min_coordinates_[depth % DIMENSIONS];
}

template <int DIMENSIONS>
bool Box<DIMENSIONS>::Contains (const Box& box) const {
    for (int k = 0; k < DIMENSIONS; ++k) {
        // first comparison is a strict inequality
        // because of the way IsBelow and IsAbove are
        // defined.
        if (!(       min_coordinates_[k] <  box.min_coordinates_[k]
              && box.max_coordinates_[k] <=     max_coordinates_[k]))
            return false;
    }
    return true;
}

template <int DIMENSIONS>
bool Box<DIMENSIONS>::Intersects (const Box *box) const {
    for (int k = 0; k < DIMENSIONS; ++k) {
        if (!(        min_coordinates_[k] <= box->max_coordinates_[k]
              && box->min_coordinates_[k] <=      max_coordinates_[k]))
            // found separating axis
            return false;
    }
    return true;
}
    
template <int DIMENSIONS>
const typename Box<DIMENSIONS>::Point& Box<DIMENSIONS>::min_coordinates() const {
    return min_coordinates_;
}

template <int DIMENSIONS>
const typename Box<DIMENSIONS>::Point& Box<DIMENSIONS>::max_coordinates() const {
    return max_coordinates_;

}

} // namespace structure
} // namespace ugdk

#endif
