#include "gtest/gtest.h"
#include <ugdk/util/indexabletable.h>

namespace {

using ugdk::util::IndexableTable;

TEST(IndexableTable, MapMethods) {
    int *a = new int, *b = new int;
	IndexableTable<int*> table;
    table.Add("first", a);
    table.Add("second", b);
    
    EXPECT_EQ(a, table.Search("first"));
    EXPECT_EQ(b, table.Search("second"));
}

struct TestData {
    int* p_;
    TestData(int* p) : p_(p) {}
    ~TestData() { --(*p_); }
};

TEST(IndexableTable, DeleteElements) {
    int count = 2;
    TestData *a = new TestData(&count), *b = new TestData(&count);
    {
        IndexableTable<TestData*> table;
        table.Add("first", a);
        table.Add("second", b);
    }
    EXPECT_EQ(0, count);
}

}