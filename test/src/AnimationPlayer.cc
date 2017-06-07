#include "gtest/gtest.h"
#include <ugdk/action/animationplayer.h>
#include <vector>

namespace {

using namespace ugdk;

struct Frame {
    Frame(int i) : id(i) {}

    int id;
    double period() const { return 1; }
};

using FrameVector = action::Animation<Frame>;
using TestTable = structure::IndexableTable<FrameVector>;

std::unique_ptr<TestTable> SetupTable() {
    auto table = MakeUnique<TestTable>();

    {
        auto v = MakeUnique<FrameVector>();
        v->frames().emplace_back(new Frame(1));
        table->Add("First", std::move(v));
    }

    {
        auto v = MakeUnique<FrameVector>();
        v->frames().emplace_back(new Frame(10));
        v->frames().emplace_back(new Frame(11));
        table->Add("Second", std::move(v));
    }

    {
        auto v = MakeUnique<FrameVector>();
        v->frames().emplace_back(new Frame(20));
        v->frames().emplace_back(new Frame(21));
        v->frames().emplace_back(new Frame(22));
        table->Add("Third", std::move(v));
    }

    return std::move(table);
}

TEST(AnimationPlayer, Getters) {
    auto table = SetupTable();
    action::AnimationPlayer<Frame> player(table.get());
    player.Select("First");
    EXPECT_EQ(table->Search("First"), player.current_animation());
    player.Refresh();
    EXPECT_EQ(1, player.current_animation_frame().id);
}


TEST(AnimationPlayer, Callback) {
    auto table = SetupTable();
    action::AnimationPlayer<Frame> player(table.get());

    int callback_called_count = 0;
    int espected_id;
    player.set_frame_change_callback([&](const Frame& frame) {
        EXPECT_EQ(espected_id, frame.id);
        callback_called_count++;
    });

    espected_id = 10;
    player.Select("Second");

    EXPECT_EQ(1, callback_called_count);
}

TEST(AnimationPlayer, Update) {
    auto table = SetupTable();
    action::AnimationPlayer<Frame> player(table.get());

    int callback_called_count = 0;
    int espected_id;
    player.set_frame_change_callback([&](const Frame& frame) {
        EXPECT_EQ(espected_id, frame.id);
        callback_called_count++;
    });

    espected_id = 10;
    player.Select("Second");
    espected_id = 11;
    player.Update(1.0);
    espected_id = 10;
    player.Update(1.0);

    EXPECT_EQ(3, callback_called_count);
}

}
