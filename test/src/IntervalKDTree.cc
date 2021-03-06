#include "gtest/gtest.h"
#include <ugdk/structure/intervalkdtree.h>
#include <ugdk/math/vector2D.h>
#include <memory>
#include <algorithm>

ugdk::structure::Box<2> makeMyBox(double a1, double a2, double b1, double b2) {
    std::array<double, 2> min, max;
    min[0] = a1;
    min[1] = a2;
    max[0] = b1;
    max[1] = b2;
    return ugdk::structure::Box<2>(min, max);
}
template<class T, class Container>
bool contains(const Container& cont, const T& t) {
    return std::find(cont.begin(), cont.end(), t) != cont.end();
}

TEST(IntervalKDTree, InsertFind) {
    using namespace ugdk::structure::ikdtree;

    IntervalKDTree<int, 2> ikd(makeMyBox(0.0, 0.0, 1.0, 1.0), 5);

    ikd.Insert(makeMyBox(0.0, 0.0, 0.5, 0.5), 1);
    ikd.Insert(makeMyBox(0.0, 0.5, 0.5, 1.0), 2);
    ikd.Insert(makeMyBox(0.8, 0.5, 0.5, 1.0), 3);

    {
        std::list<int> v;
        ikd.FindIntersectingItems(makeMyBox(0.25, 0.25, 0.75, 0.75), std::back_inserter(v));
        EXPECT_TRUE(contains(v, 1));
        EXPECT_TRUE(contains(v, 2));
        EXPECT_FALSE(contains(v, 3));
        EXPECT_EQ(2u, v.size());
    }

    ikd.Update(makeMyBox(0.5, 0.5, 0.5, 1.0), 3);
    {
        std::list<int> v;
        ikd.FindIntersectingItems(makeMyBox(0.25, 0.25, 0.75, 0.75), std::back_inserter(v));
        EXPECT_TRUE(contains(v, 1));
        EXPECT_TRUE(contains(v, 2));
        EXPECT_TRUE(contains(v, 3));
        EXPECT_EQ(3u, v.size());
    }
}

