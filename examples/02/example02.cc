#include <ugdk/system/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/input/module.h>
#include <ugdk/input/events.h>
#include <ugdk/input/scancode.h>
#include <ugdk/action/entity.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/canvas.h>
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
        if(manager->keyboard().IsDown(input::Scancode::A))
            MoveLeft(dt);
        if(manager->keyboard().IsDown(input::Scancode::D))
            MoveRight(dt);
        if(manager->keyboard().IsDown(input::Scancode::W))
            MoveUp(dt);
        if(manager->keyboard().IsDown(input::Scancode::S))
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

    scene->event_handler().AddListener<input::KeyPressedEvent>(
        [scene](const input::KeyPressedEvent& ev) -> void {
            if(ev.scancode == input::Scancode::ESCAPE)
                scene->Finish();
        });
    scene->AddEntity(r);
    scene->set_render_function([r](graphic::Canvas& canvas) {
        canvas.PushAndCompose(graphic::Geometry(r->position()));
        r->drawable()->Draw(canvas);
        canvas.PopGeometry();
    });

    system::PushScene(scene);
    system::Run();
    system::Release();
    return 0;
}