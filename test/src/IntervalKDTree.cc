#include "gtest/gtest.h"
#include <ugdk/util/intervalkdtree.h>
#include <memory>
#include <algorithm>

ugdk::ikdtree::Box<2> makeMyBox(double a1, double a2, double b1, double b2) {
    double min[] = { a1, a2 }, max[] = { b1, b2 };
    return ugdk::ikdtree::Box<2>(min, max);
}
template<class T, class Container>
bool contains(const Container& cont, const T& t) {
    return std::find(cont.begin(), cont.end(), t) != cont.end();
}

TEST(IntervalKDTree, InsertFind) {
    using namespace ugdk::ikdtree;

    IntervalKDTree<int, 2> ikd(makeMyBox(0.0, 0.0, 1.0, 1.0), 5);

    ikd.Insert(makeMyBox(0.0, 0.0, 0.5, 0.5), 1);
    ikd.Insert(makeMyBox(0.0, 0.5, 0.5, 1.0), 2);
    ikd.Insert(makeMyBox(0.8, 0.5, 0.5, 1.0), 3);

    {
        std::shared_ptr<std::vector<int>> v(ikd.getIntersectingItems(makeMyBox(0.25, 0.25, 0.75, 0.75)));
        EXPECT_TRUE(contains(*v, 1));
        EXPECT_TRUE(contains(*v, 2));
        EXPECT_FALSE(contains(*v, 3));
        EXPECT_EQ(2, v->size());
    }

    ikd.Update(makeMyBox(0.5, 0.5, 0.5, 1.0), 3);
    {
        std::shared_ptr<std::vector<int>> v(ikd.getIntersectingItems(makeMyBox(0.25, 0.25, 0.75, 0.75)));
        EXPECT_TRUE(contains(*v, 1));
        EXPECT_TRUE(contains(*v, 2));
        EXPECT_TRUE(contains(*v, 3));
        EXPECT_EQ(3, v->size());
    }
}

