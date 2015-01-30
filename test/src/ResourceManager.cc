#include "gtest/gtest.h"

#include <ugdk/resource/manager.h>
#include <ugdk/resource/resourcecontainer.h>

namespace resourcemanager_test {
    static int zero = 0;

    template<class Dummy>
    struct DummyContainer : ugdk::resource::ResourceContainer<Dummy> {

        static Dummy& default_value() {
            static Dummy value;
            return value;
        }

        void Insert(const std::string& tag, Dummy val) override {}
        void Replace(const std::string& tag, Dummy val) override {}
        bool Exists(const std::string& tag) const override { return false; }
        Dummy&   Find(const std::string& tag) override { return default_value(); }
        Dummy&   Load(const std::string& filepath, const std::string& tag) override { return default_value(); }
    };

    using A = DummyContainer < int > ;
    using B = DummyContainer < double >;
    using C = DummyContainer < std::string >;
}

TEST(ResourceManager, AddGetOneResource) {
    using namespace resourcemanager_test;
    A* a = new A;

    ugdk::resource::Manager manager;
    manager.AddContainer<int>(std::unique_ptr<A>(a));

    EXPECT_EQ(a, manager.GetContainer<int>());
}

TEST(ResourceManager, AddGetManyResource) {
    using namespace resourcemanager_test;
    A* a = new A;
    B* b = new B;
    C* c = new C;

    ugdk::resource::Manager manager;
    manager.AddContainer<int>(std::unique_ptr<A>(a));
    manager.AddContainer<double>(std::unique_ptr<B>(b));
    manager.AddContainer<std::string>(std::unique_ptr<C>(c));

    EXPECT_EQ(a, manager.GetContainer<int>());
    EXPECT_EQ(b, manager.GetContainer<double>());
    EXPECT_EQ(c, manager.GetContainer<std::string>());
}
