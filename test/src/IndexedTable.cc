#include "gtest/gtest.h"
#include <ugdk/util/indexabletable.h>

namespace {

using ugdk::util::IndexableTable;

TEST(IndexableTable, MapMethods) {
    int *a = new int, *b = new int, *c = new int;
    IndexableTable<int*> table;
    table.Add("first", a);
    table.Add("second", b);
    
    table.Add("third", c);
    table.Remove("third");
    
    EXPECT_EQ(a, table.Search("first"));
    EXPECT_EQ(b, table.Search("second"));
    EXPECT_EQ(NULL, table.Search("third"));
    
}

struct TestData {
    int* p_;
    TestData(int* p) : p_(p) {}
    ~TestData() { --(*p_); }
};

TEST(IndexableTable, DeleteElements) {
    int count = 3;
    TestData *a = new TestData(&count), *b = new TestData(&count),
        *c = new TestData(&count);
    {
        IndexableTable<TestData*> table;
        table.Add("first", a);
        table.Add("second", b);
        table.Add("third", c);
        
        table.Remove("third");
        EXPECT_EQ(2, count);
    }
    EXPECT_EQ(0, count);
}

TEST(IndexableTable, Id) {
    int *a = new int, *b = new int;
    IndexableTable<int*> table;
    table.Add("first", a);
    table.Add("second", b);
    
    int id1 = table.MakeIndex("first");
    int id2 = table.MakeIndex("second");
    EXPECT_EQ(-1, table.MakeIndex("third"));
    
    EXPECT_EQ(a, table.Get(id1));
    EXPECT_EQ(b, table.Get(id2));
}

TEST(IndexableTable, InvalidOperation) {
    {
        IndexableTable<int*> table;
        table.Add("null", NULL);
        EXPECT_FALSE(table.ReleaseIndex(5));
    }
}

}