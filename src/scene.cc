#include "scene.h"

namespace ugdk {

using namespace std;

Scene::~Scene() {
    if (layers_.size() == 0) return;
    list<Layer*>::iterator it = layers_.begin();
    while (it != layers_.end()) {
        delete (*it);
        ++it;
    }
}

void Scene::Update(float delta_t) {

    if (layers_.size() == 0) return;

    list<Layer*>::iterator it = layers_.begin();

    while (it != layers_.end()) {
        (*it)->Update(delta_t);
        ++it;
    }
}

void Scene::Render() {
    if (visible_) {
        list<Layer*>::iterator it = layers_.begin();
        while (it != layers_.end()) {
            (*it)->Render();
            ++it;
        }
    }
}

void Scene::RenderLight() {
    if (visible_) {
        list<Layer*>::iterator it = layers_.begin();
        while (it != layers_.end()) {
            (*it)->RenderLight();
            ++it;
        }
    }
}

}
