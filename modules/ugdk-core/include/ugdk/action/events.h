#ifndef UGDK_ACTION_EVENTS_H_
#define UGDK_ACTION_EVENTS_H_

#include <ugdk/action.h>

namespace ugdk {
namespace action {

struct SceneFocusEvent {
    Scene* scene;

    SceneFocusEvent(Scene* _scene) : scene(_scene) {}
};

struct SceneDefocusEvent {
    Scene* scene;

    SceneDefocusEvent(Scene* _scene) : scene(_scene) {}
};

struct SceneFinishedEvent {
    Scene* scene;

    SceneFinishedEvent(Scene* _scene) : scene(_scene) {}
};

} // namespace action
} // namespace ugdk

#endif // UGDK_ACTION_EVENTS_H_
