#include <ugdk/action/scene.h>

#include <ugdk/action/events.h>
#include <ugdk/audio/module.h>
#include <ugdk/audio/music.h>
#include <ugdk/debug/profiler.h>

#include <algorithm>
#include <vector>

namespace ugdk {
namespace action {

using std::list;

Scene::Scene()
  : active_(true)
  , finished_(false)
  , visible_(true)
  , background_music_(nullptr)
  , stops_previous_music_(true)
  , event_handler_()
{
    AddTask(system::Task([&](double dt) {
        media_manager_.Update(dt);
    }, 0.4));
}

Scene::~Scene() {}

void Scene::Focus() {
    event_handler_.RaiseEvent(SceneFocusEvent(this));
    if(background_music_ != nullptr) {
        if(!background_music_->IsPlaying())
            background_music_->PlayForever();
        else
            background_music_->Unpause();

    } else if(stops_previous_music_) {
        std::shared_ptr<audio::Music> current_music = audio::manager().CurrentMusic();
        if(current_music != nullptr)
            current_music->Stop();
    }
}

void Scene::DeFocus() {
    event_handler_.RaiseEvent(SceneDefocusEvent(this));
}

void Scene::Finish() {
    if (finished_)
        return;
    End();
    event_handler_.RaiseEvent(SceneFinishedEvent(this));
    finished_ = true;
}

void Scene::Update(double dt) {
    if(finished_ || !active_)
        return;

    debug::ProfileSection section("Scene '" + identifier_ + "'");
    TaskPlayer::Update(dt);
}

void Scene::Render(std::vector<graphic::Canvas*>& canvases) const {
    if(!finished_ && visible_ && render_function_) {
        debug::ProfileSection section("Scene '" + identifier_ + "'");
        render_function_(canvases);
    }
}

void Scene::End() {
    if(background_music_ != nullptr)
        background_music_->Pause();
}

} /* namespace action */
} /* namespace ugdk */
