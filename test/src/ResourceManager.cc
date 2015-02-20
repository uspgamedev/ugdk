#include "gtest/gtest.h"

#include <ugdk/resource/manager.h>
#include <ugdk/resource/resourcecontainer.h>

#include <functional>

namespace resourcemanager_test {
    class First {};

    class CallOnDestroy {
    public:
        CallOnDestroy(std::function<void(void)> f) : f_(f) {}
        ~CallOnDestroy() { f_(); }

    private:
        std::function<void(void)> f_;
    };
}

TEST(ResourceManager, CreateContainer) {
    ugdk::resource::Manager manager;
    manager.CreateContainer<resourcemanager_test::First>(&ugdk::resource::NullLoad<resourcemanager_test::First>);

    EXPECT_TRUE(manager.GetContainer<resourcemanager_test::First>() != nullptr);
}

TEST(ResourceManager, Destructor) {
    using namespace resourcemanager_test;

    bool destroyed_ = false;
    {
        ugdk::resource::Manager manager;
        manager.CreateContainer<CallOnDestroy>(&ugdk::resource::NullLoad<CallOnDestroy>);
        ASSERT_TRUE(manager.GetContainer<CallOnDestroy>() != nullptr);
        manager.GetContainer<CallOnDestroy>()->Insert("Random", new CallOnDestroy([&] {
            destroyed_ = true;
        }));
    }
    EXPECT_TRUE(destroyed_);
}
