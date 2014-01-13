#include <ugdk/graphic/manager.h>

#include <ugdk/action/scene.h>
#include <ugdk/graphic/defaultshaders.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/opengl/shaderprogram.h>

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
    CreateLightBuffer(math::Vector2D(canvas_->size()));

    shaders_.ReplaceShader(0, CreateShader(false, false));
    shaders_.ReplaceShader(1, CreateShader( true, false));
    shaders_.ReplaceShader(2, CreateShader(false,  true));
    shaders_.ReplaceShader(3, CreateShader( true,  true));

    light_shader_ = LightShader();

    unsigned char buffer[32*32*4];
    for(int i = 0; i < 32*32*4; ++i) buffer[i] = 255;
    if(white_texture_) delete white_texture_;
    white_texture_ = Texture::CreateRawTexture(32, 32);
    glBindTexture(GL_TEXTURE_2D, white_texture_->gltexture());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Manager::Release() {
    canvas_.reset();
}

action::Scene* CreateLightrenderingScene(std::function<void (graphic::Canvas&)> render_light_function) {
    action::Scene* light_scene = new action::Scene;
    light_scene->set_identifier("Light Rendering Scene");
    // This scene has no logic, so quit if you ask for it to be only scene.
    light_scene->set_focus_callback([](action::Scene* scene) { scene->Finish(); });
    light_scene->set_render_function([render_light_function](graphic::Canvas& canvas) {
        graphic::Manager* manager = graphic::manager();

        canvas.Clear(Color(0.0, 0.0, 0.0, 0.0));
    
        // Lights are simply added together.
        glBlendFunc(GL_ONE, GL_ONE);

        // Draw the lights, as the user specified.
        render_light_function(canvas);

        // Draw all lights to a buffer, merging then to a light texture.
        canvas.SaveToTexture(manager->light_buffer());
    
        // Clear the screen so it's back to how it was before.
        canvas.Clear(Color(0.0, 0.0, 0.0, 0.0));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Bind the light texture to all shaders that USE_LIGHT_BUFFER.
        opengl::ShaderProgram::Use(manager->shaders().GetSpecificShader((1 << 0) + (0 << 1)))
            .SendTexture(1, manager->light_buffer(), "light_texture");
        opengl::ShaderProgram::Use(manager->shaders().GetSpecificShader((1 << 0) + (1 << 1)))
            .SendTexture(1, manager->light_buffer(), "light_texture");
    });

    return light_scene;
}

void Manager::CreateLightBuffer(const math::Vector2D& size) {
    if(light_buffer_ != nullptr) delete light_buffer_;
    light_buffer_ = Texture::CreateRawTexture(size.x, size.y);
    glBindTexture(GL_TEXTURE_2D, light_buffer_->gltexture());
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
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
