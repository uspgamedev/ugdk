#include "gtest/gtest.h"
#include <ugdk/structure/indexabletable.h>
#include <ugdk/system/compatibility.h>

namespace {

using ugdk::structure::IndexableTable;

TEST(IndexableTable, MapMethods) {
    int *a = new int;
    int *b = new int;
    int *c = new int;
    IndexableTable<int*> table;
    table.Add("first", std::unique_ptr<int>(a));
    table.Add("second", std::unique_ptr<int>(b));
    
    table.Add("third", std::unique_ptr<int>(c));
    table.Remove("third");
    
    EXPECT_EQ(2u, table.size());
    EXPECT_EQ(a, table.Search("first"));
    EXPECT_EQ(b, table.Search("second"));
    EXPECT_EQ(nullptr, table.Search("third"));
    
}

struct TestData {
    int* p_;
    TestData(int* p) : p_(p) {}
    ~TestData() { --(*p_); }
};

TEST(IndexableTable, DeleteElements) {
    int count = 3;
    {
        IndexableTable<TestData*> table;
        table.Add("first", ugdk::MakeUnique<TestData>(&count));
        table.Add("second", ugdk::MakeUnique<TestData>(&count));
        table.Add("third", ugdk::MakeUnique<TestData>(&count));
        
        table.Remove("third");
        EXPECT_EQ(2, count);
    }
    EXPECT_EQ(0, count);
}

TEST(IndexableTable, Id) {
    int *a = new int; 
    int *b = new int;
    IndexableTable<int*> table;
    table.Add("first", std::unique_ptr<int>(a));
    table.Add("second", std::unique_ptr<int>(b));
    
    int id1 = table.MakeIndex("first");
    int id2 = table.MakeIndex("second");
    EXPECT_EQ(-1, table.MakeIndex("third"));
    
    EXPECT_EQ(a, table.Get(id1));
    EXPECT_EQ(b, table.Get(id2));
}

TEST(IndexableTable, InvalidOperation) {
    {
        IndexableTable<int*> table;
        table.Add("null", nullptr);
        EXPECT_FALSE(table.ReleaseIndex(5));
    }
}

}
