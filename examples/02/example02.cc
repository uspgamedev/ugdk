#include <ugdk/system/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/input/module.h>
#include <ugdk/action/entity.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/module.h>

using namespace ugdk;

class Rectangle : public action::Entity {
  public:
    Rectangle() 
      : velocity_(500.0), 
        drawable_(new graphic::TexturedRectangle(graphic::manager()->white_texture(), math::Vector2D(50.0, 50.0))) {
    }
    ~Rectangle() {
        delete drawable_;
    }

    void Update(double dt) override {
        auto manager = input::manager();
        if(manager->KeyDown(input::K_a))
            MoveLeft(dt);
        if(manager->KeyDown(input::K_d))
            MoveRight(dt);
        if(manager->KeyDown(input::K_w))
            MoveUp(dt);
        if(manager->KeyDown(input::K_s))
            MoveDown(dt);
    }
    
    void MoveLeft(double dt) {
        position_.x -= dt*velocity_;
    }
    void MoveRight(double dt) {
        position_.x += dt*velocity_;
    }
    void MoveUp(double dt) {
        position_.y -= dt*velocity_;
    }
    void MoveDown(double dt) {
        position_.y += dt*velocity_;
    }

    const math::Vector2D& position() const { return position_; }
    graphic::TexturedRectangle* drawable() { return drawable_; }

  private:
    math::Vector2D position_;
    double velocity_;
    graphic::TexturedRectangle* drawable_;
};

int main(int argc, char* argv[]) {
    system::Initialize();
    action::Scene* scene = new action::Scene();
    Rectangle* r = new Rectangle;

    scene->AddTask([scene](double dt) {
        if(input::manager()->KeyDown(input::K_ESCAPE))
            scene->Finish();
    });
    scene->AddEntity(r);
    scene->set_render_function([r](const graphic::Geometry& geometry, const graphic::VisualEffect& effect) {
        r->drawable()->Draw(geometry * graphic::Geometry(r->position()), effect);
    });

    system::PushScene(scene);
    system::Run();
    system::Release();
    return 0;
}