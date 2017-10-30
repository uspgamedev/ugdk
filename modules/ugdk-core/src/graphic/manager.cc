﻿#include <ugdk/graphic/manager.h>

#include <ugdk/action/scene.h>
#include <ugdk/action/events.h>
#include <ugdk/desktop/window.h>
#include <ugdk/graphic/defaultshaders.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/shaderprogram.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/rendertexture.h>
#include <ugdk/debug/profiler.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/util/idgenerator.h>
#include <ugdk/graphic/exceptions.h>

#include "gltexture.h"
#include "SDL_video.h"

namespace ugdk {
namespace graphic {

class RenderScreen : public RenderTarget {
public:
    math::Vector2D size() const override {
        return size_;
    }

    void Resize(const math::Vector2D& canvas_size) {
        size_ = canvas_size;
        projection_matrix_ = math::Geometry(math::Vector2D(-1.0, 1.0), math::Vector2D(2.0/size_.x, -2.0/size_.y));
    }

    void SaveToTexture(graphic::GLTexture* texture) {
        glBindTexture(GL_TEXTURE_2D, texture->id());
        //glReadBuffer(GL_BACK); FIXME
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texture->width(), texture->height());
        internal::AssertNoOpenGLError();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void AttachTo(const std::weak_ptr<desktop::Window>& weak_window) {
        window_ = weak_window;
    }

    void UpdateViewport() {
        if(auto window = window_.lock())
            glViewport(0, 0, window->size().x, window->size().y);
    }
    std::weak_ptr<desktop::Window> Window() {
        return window_;
    }
private:
    std::weak_ptr<desktop::Window> window_;
    math::Vector2D size_;
};

Manager::Manager()
    :   light_buffer_(nullptr)
    ,   white_texture_(nullptr)
    ,   light_shader_(nullptr)
    ,   active_index_(0)
    {}

Manager::~Manager() {}

void Manager::RegisterScreen(std::weak_ptr<desktop::Window> weak_window) {
    auto screen_ptr = std::make_unique<RenderScreen>();
    screen_ptr->AttachTo(weak_window);
    screens_.emplace_back(std::move(screen_ptr));
}

void Manager::UseCanvas(graphic::Canvas &canvas) {
    canvas.Bind();
}

void Manager::SetActiveScreen(uint32_t index) {

    screen(active_index_)->Unbind();
    screen(index)->Bind();
    active_index_ = index;

    SDL_GL_MakeCurrent(
                       dynamic_cast<RenderScreen*>(screen(index))->Window().lock()->sdl_window_,
                       context_
                      ); //FIXME?
}

void Manager::ResizeScreen(uint32_t index, const math::Vector2D& canvas_size) {
    screens_[index]->Resize(canvas_size);
}

void Manager::SetUserNearestNeighborTextures(bool enabled) {
    if (enabled) {
        GLTexture::set_texture_filter(GL_NEAREST);
    } else {
        GLTexture::set_texture_filter(GL_LINEAR);
    }
}

bool Manager::Initialize(
            const std::vector<std::weak_ptr<desktop::Window>>& windows_, 
            const math::Vector2D& canvas_size
    ) {
    
    if (windows_.size()==0)
        return false;
    
    for (auto _window_ : windows_)
        RegisterScreen(_window_);

    context_ = SDL_GL_CreateContext(windows_[0].lock().get()->sdl_window_);//Set primary window  //FIXME?
    if(!context_)
        return false; //errlog("OpenGL context creation failed: " + string(SDL_GetError()));

#ifndef UGDK_USING_GLES
    GLenum err = glewInit();
    if (GLEW_OK != err)
        return false; //errlog("GLEW Error: " + string((const char*)(glewGetErrorString(err))));
#endif
    for (uint32_t i = 0; i < screens_.size(); i++) {
        //screens_[i].reset(new RenderScreen);
        ResizeScreen(i, canvas_size);
    }
    //SetActiveScreen(0);

    // This hint can improve the speed of texturing when perspective-correct texture
    // coordinate interpolation isn't needed, such as when using a glOrtho() projection.
    // From http://www.mesa3d.org/brianp/sig97/perfopt.htm
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST); FIXME

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    internal::AssertNoOpenGLError();

    int max_combined_texture_image_units;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_combined_texture_image_units);
    textureunit_ids_.reset(new util::IDGenerator(0, max_combined_texture_image_units, -1));

    light_buffer_.reset(new RenderTexture(math::Integer2D(canvas_size)));

    shaders_.ReplaceShader(0, CreateShader(false, false));
    shaders_.ReplaceShader(1, CreateShader( true, false));
    shaders_.ReplaceShader(2, CreateShader(false,  true));
    shaders_.ReplaceShader(3, CreateShader( true,  true));

    light_shader_ = LightShader();

    unsigned char buffer[32*32*4];
    for(int i = 0; i < 32*32*4; ++i) buffer[i] = 255;
    if(white_texture_) delete white_texture_;
    white_texture_ = graphic::GLTexture::CreateRawTexture(32, 32);

    glBindTexture(GL_TEXTURE_2D, white_texture_->id());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    internal::AssertNoOpenGLError();

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Manager::Release() {
    SDL_GL_DeleteContext(context_);
    screens_[0].reset();
    light_buffer_.reset();
    textureunit_ids_.reset();
}

TextureUnit Manager::ReserveTextureUnit(const graphic::GLTexture* texture) {
    int id = textureunit_ids_->GenerateID();
    if (id == textureunit_ids_->error_value())
        throw NotSupportedException("Maximum amount of TextureUnits reached.");

    TextureUnit unit(id);
    if (texture)
        unit.BindTexture(texture);
    return unit;
}

void Manager::ReleaseTextureUnitID(int id) {
    textureunit_ids_->ReleaseID(id);
}

void Manager::DisableVertexType(VertexType type) {
    glDisableVertexAttribArray(LocationForVertexType(type));
}

unsigned int Manager::LocationForVertexType(VertexType type) {
    switch(type) {
    case VertexType::VERTEX:  return 0;
    case VertexType::TEXTURE: return 1;
    case VertexType::COLOR:   return 2;
    case VertexType::CUSTOM1: return 3;
    }
    return -1u; /* It should never get here, but this keeps gcc quiet, so I did it */
}

system::FunctionListener<action::SceneFocusEvent> quit_event = [](const action::SceneFocusEvent& ev) {
    ev.scene->Finish();
}; 
/*
action::Scene* CreateLightrenderingScene(std::function<void (graphic::Canvas&)> render_light_function) {
    action::Scene* light_scene = new action::Scene;
    light_scene->set_identifier("Light Rendering Scene");
    // This scene has no logic, so quit if you ask for it to be only scene.
    light_scene->event_handler().AddListener(quit_event);
    light_scene->set_render_function(
        light_scene->num_functions(),
        [render_light_function](graphic::Canvas& canvases) {
        graphic::Manager& manager = graphic::manager();
        Canvas light_canvas(manager.light_buffer());

        // Lights are simply added together.
        glBlendFunc(GL_ONE, GL_ONE);

        light_canvas.Clear(structure::Color(0.0, 0.0, 0.0, 0.0));
        render_light_function(light_canvas);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    });

    return light_scene;
}*/

const ShaderProgram* Manager::Shaders::current_shader() const {
    return shaders_[flags_.to_ulong()];
}

const ShaderProgram* Manager::Shaders::GetSpecificShader(const std::bitset<Manager::Shaders::NUM_FLAGS>& flags) const {
    return shaders_[flags.to_ulong()];
}

bool Manager::Shaders::IsFlagSet(Flag flag) const {
    int flag_bit = static_cast<int>(flag);
    return flags_[flag_bit];
}

void Manager::Shaders::ChangeFlag(Flag flag, bool value) {
    int flag_bit = static_cast<int>(flag);
    flags_[flag_bit] = value;
}

void Manager::Shaders::ReplaceShader(const std::bitset<NUM_FLAGS>& flags, ShaderProgram* program) {
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

RenderTarget* Manager::screen(uint32_t index) const {
    return screens_[index].get();
}
uint32_t Manager::num_screens() {
    return screens_.size();
}

}  // namespace graphic
}  // namespace ugdk
