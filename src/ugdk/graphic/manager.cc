#include <ugdk/graphic/manager.h>

#include <cmath>
#include <cassert>

#include "GL/glew.h"
#ifdef _WIN32
#   include "windows.h"
#   include "GL/wglew.h"
#endif
#define NO_SDL_GLEXT
#include "SDL.h"
#include "SDL_image.h"

#include <ugdk/action/scene.h>
#include <ugdk/graphic/defaultshaders.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/opengl/shaderprogram.h>

#define LN255 5.5412635451584261462455391880218

namespace ugdk {
namespace graphic {

using std::string;

namespace {
bool errlog(const string& msg) {
    fprintf(stderr, "ugdk::graphic::Manager Error Log - %s\n", msg.c_str());
    return false;
}
}

static ugdk::math::Vector2D default_resolution(800.0, 600.0);
    
Manager::Manager() 
    :   light_buffer_(nullptr)
    ,   white_texture_(nullptr)
    ,   light_shader_(nullptr) {}

bool Manager::Initialize() {
    // Changing to and from fullscreen destroys all textures, so we must recreate them.
    CreateLightBuffer();

    shaders_.ReplaceShader(0, CreateShader(false, false));
    shaders_.ReplaceShader(1, CreateShader( true, false));
    shaders_.ReplaceShader(2, CreateShader(false,  true));
    shaders_.ReplaceShader(3, CreateShader( true,  true));

    light_shader_ = LightShader();
    return true;
}

void Manager::Release() {}

action::Scene* CreateLightrenderingScene(std::function<void (const graphic::Geometry&, const graphic::VisualEffect&)> render_light_function) {
    action::Scene* light_scene = new action::Scene;
    // This scene has no logic, so quit if you ask for it to be only scene.
    light_scene->set_focus_callback([](action::Scene* scene) { scene->Finish(); });
    light_scene->set_render_function([render_light_function](const graphic::Geometry& geometry, const graphic::VisualEffect& effect) {
        graphic::Manager* manager = graphic::manager();

        glClearColor( 0.0, 0.0, 0.0, 0.0 );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
        // Lights are simply added together.
        glBlendFunc(GL_ONE, GL_ONE);

        // Draw the lights, as the user specified.
        render_light_function(geometry, effect);

        // Draw all lights to a buffer, merging then to a light texture.
        manager->SaveBackbufferToTexture(manager->light_buffer());
    
        // Clear the screen so it's back to how it was before.
        glClearColor( 0.0, 0.0, 0.0, 0.0 );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Bind the light texture to all shaders that USE_LIGHT_BUFFER.
        opengl::ShaderProgram::Use(manager->shaders().GetSpecificShader((1 << 0) + (0 << 1)))
            .SendTexture(1, manager->light_buffer(), "light_texture");
        opengl::ShaderProgram::Use(manager->shaders().GetSpecificShader((1 << 0) + (1 << 1)))
            .SendTexture(1, manager->light_buffer(), "light_texture");
    });

    return light_scene;
}

// Desenha backbuffer na tela
void Manager::Render(const std::list<action::Scene*>& scene_list) {

    for(action::Scene* it : scene_list)
        it->Render(initial_geometry_, VisualEffect());

    // Swap the buffers to show the backbuffer to the user.
    SDL_GL_SwapWindow(window_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Manager::initializeLight() {
    if(light_buffer_ != nullptr) delete light_buffer_;
    light_buffer_ = Texture::CreateRawTexture(settings_.resolution.x, settings_.resolution.y);
    glBindTexture(GL_TEXTURE_2D, light_buffer_->gltexture());
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, settings_.resolution.x, 
        settings_.resolution.y, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned char buffer[32*32*4];
    for(int i = 0; i < 32*32*4; ++i) buffer[i] = 255;
    if(white_texture_) delete white_texture_;
    white_texture_ = Texture::CreateRawTexture(32, 32);
    glBindTexture(GL_TEXTURE_2D, white_texture_->gltexture());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
