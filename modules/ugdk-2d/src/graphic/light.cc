#include <ugdk/graphic/light.h>

#include <ugdk/graphic/opengl.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/structure/visualeffect.h>

namespace ugdk {
namespace graphic {

namespace {
    double kSQUARE_SIZE = 4.0;
    struct VertexXY {
        float x, y;

        void set_xy(double _x, double _y) {
            this->x = static_cast<float>(_x);
            this->y = static_cast<float>(_y);
        }
    };
}

Light::Light()
    : dimension_(1.0, 1.0)
    , data_(4, sizeof(VertexXY), false)
{
    VertexData::Mapper mapper(data_);
    mapper.Get<VertexXY>(0)->set_xy(-kSQUARE_SIZE, -kSQUARE_SIZE);
    mapper.Get<VertexXY>(1)->set_xy(kSQUARE_SIZE, -kSQUARE_SIZE);
    mapper.Get<VertexXY>(2)->set_xy(-kSQUARE_SIZE, kSQUARE_SIZE);
    mapper.Get<VertexXY>(3)->set_xy(kSQUARE_SIZE, kSQUARE_SIZE);
}

Light::~Light() {}

void Light::set_dimension(const ugdk::math::Vector2D& dimension) {
    dimension_ = dimension;
}

void Light::Draw(Canvas &canvas) {
    const ShaderProgram* old_program = canvas.shader_program();

    canvas.ChangeShaderProgram(graphic::manager().light_shader());
    canvas.SendUniform("decayment", 2.4f);
    canvas.SendUniform("minimum_radius", 0.05f);
    canvas.SendUniform("dimension", dimension_.x, dimension_.y);
    canvas.PushAndCompose(structure::VisualEffect(color_));
    canvas.SendVertexData(data_, VertexType::VERTEX, 0);
    graphic::manager().DisableVertexType(VertexType::TEXTURE);
    canvas.DrawArrays(DrawMode::TRIANGLE_STRIP(), 0, 4);

    canvas.PopVisualEffect();
    canvas.ChangeShaderProgram(old_program);
}

}  // namespace graphic
}  // namespace ugdk
