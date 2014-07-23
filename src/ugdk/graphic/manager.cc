#include <ugdk/graphic/manager.h>

#include <ugdk/action/scene.h>
#include <ugdk/internal/gltexture.h>
#include <ugdk/graphic/defaultshaders.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/graphic/framebuffer.h>
#include <ugdk/debug/profiler.h>
#include <ugdk/math/integer2D.h>

#include "SDL_video.h"

namespace ugdk {
namespace graphic {

Manager::Manager(const std::weak_ptr<desktop::Window>& window, const math::Vector2D& canvas_size)
    :   canvas_(Canvas::Create(window, canvas_size))
    ,   light_buffer_(nullptr)
    ,   white_texture_(nullptr)
    ,   light_shader_(nullptr) {}

Manager::~Manager() {}

bool Manager::Initialize() {
    light_buffer_ = Framebuffer::Create(canvas_->size());

    shaders_.ReplaceShader(0, CreateShader(false, false));
    shaders_.ReplaceShader(1, CreateShader( true, false));
    shaders_.ReplaceShader(2, CreateShader(false,  true));
    shaders_.ReplaceShader(3, CreateShader( true,  true));

    light_shader_ = LightShader();

    unsigned char buffer[32*32*4];
    for(int i = 0; i < 32*32*4; ++i) buffer[i] = 255;
    if(white_texture_) delete white_texture_;
    white_texture_ = internal::GLTexture::CreateRawTexture(32, 32);

    glBindTexture(GL_TEXTURE_2D, white_texture_->id());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    internal::AssertNoOpenGLError();

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Manager::Release() {
    canvas_.reset();
    light_buffer_.reset();
}

action::Scene* CreateLightrenderingScene(std::function<void (graphic::Canvas&)> render_light_function) {
    action::Scene* light_scene = new action::Scene;
    light_scene->set_identifier("Light Rendering Scene");
    // This scene has no logic, so quit if you ask for it to be only scene.
    light_scene->set_focus_callback([](action::Scene* scene) { scene->Finish(); });
    light_scene->set_render_function([render_light_function](graphic::Canvas& canvas) {
        graphic::Manager* manager = graphic::manager();
        auto light_buffer = manager->light_buffer();

        // Lights are simply added together.
        glBlendFunc(GL_ONE, GL_ONE);

        light_buffer->Bind();
        light_buffer->Clear(Color(0.0, 0.0, 0.0, 0.0));
        render_light_function(canvas);
        light_buffer->Unbind();
    
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Bind the light texture to all shaders that USE_LIGHT_BUFFER.
        opengl::ShaderUse(manager->shaders().GetSpecificShader((1 << 0) + (0 << 1)))
            .SendTexture(1, light_buffer->texture(), "light_texture");
        opengl::ShaderUse(manager->shaders().GetSpecificShader((1 << 0) + (1 << 1)))
            .SendTexture(1, light_buffer->texture(), "light_texture");
    });

    return light_scene;
}

const opengl::ShaderProgram* Manager::Shaders::current_shader() const {
    return shaders_[flags_.to_ulong()];
}
        
const opengl::ShaderProgram* Manager::Shaders::GetSpecificShader(const std::bitset<Manager::Shaders::NUM_FLAGS>& flags) const {
    return shaders_[flags.to_ulong()];
}

void Manager::Shaders::ChangeFlag(Flag flag, bool value) {
    int flag_bit = static_cast<int>(flag);
    flags_[flag_bit] = value;
}

void Manager::Shaders::ReplaceShader(const std::bitset<NUM_FLAGS>& flags, opengl::ShaderProgram* program) {
    delete shaders_[flags.to_ulong()];
    shaders_[flags.to_ulong()] = program;
}

Manager::Shaders::Shaders() {
    unsigned long max_flags = 1 << NUM_FLAGS;
    for(unsigned long i = 0; i < max_flags; ++i)
        shaders_[i] = nullptr;
}
        
Manager::Shaders::~Shaders() {
    unsigned long max_flags = 1 << NUM_FLAGS;
    for(unsigned long i = 0; i < max_flags; ++i)
        delete shaders_[i];
}

}  // namespace graphic
}  // namespace ugdk
