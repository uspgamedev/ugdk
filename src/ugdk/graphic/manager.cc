#include <ugdk/graphic/manager.h>

#include <cmath>
#include <cassert>

#include "GL/glew.h"
#ifdef _WIN32
#   include "GL/wglew.h"
#endif
#define NO_SDL_GLEXT
#include "SDL.h"
#include "SDL_opengl.h"

#include <ugdk/action/scene.h>
#include <ugdk/graphic/defaultshaders.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/opengl/shaderprogram.h>

#define LN255 5.5412635451584261462455391880218

namespace ugdk {
namespace graphic {

using std::string;

static ugdk::math::Vector2D default_resolution(800.0, 600.0);
    
VideoSettings::VideoSettings()
    : fullscreen(false), vsync(true), light_system(false) {}

Manager::Manager(const VideoSettings& settings) 
    :   settings_(settings), 
        light_buffer_(nullptr), 
        white_texture_(nullptr), 
        light_shader_(nullptr) {}

bool Manager::Initialize() {
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        return false;
    
    // Set window title.
    SDL_WM_SetCaption(settings_.window_title.c_str(), 
                      (settings_.window_icon.length() > 0) ? settings_.window_icon.c_str() : nullptr );
    
    if(settings_.window_icon.length() > 0)
        SDL_WM_SetIcon(SDL_LoadBMP(settings_.window_icon.c_str()), nullptr);
       
    if(UpdateResolution() == false) {
        /* TODO: insert error message here. */
        return false;
    }

    UpdateVSync();

    glClearColor( 0.0, 0.0, 0.0, 0.0 );

    /*if(GLEW_ARB_framebuffer_object) {
        glGenFramebuffersEXT(1, &light_buffer_id_);
    }*/
    return true;
}

bool Manager::ChangeSettings(const VideoSettings& new_settings) {
    VideoSettings old_settings = settings_;

    settings_ = new_settings;

    if(old_settings.resolution != new_settings.resolution || 
        old_settings.fullscreen != new_settings.fullscreen) {
        if(!UpdateResolution())
            return false;
    }

    UpdateVSync();

    return true;
}

// Changes the resolution to the requested value.
// Returns true on success.
bool Manager::UpdateResolution() {
    Uint32 flags = SDL_OPENGL;
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    if(settings_.fullscreen) flags |= SDL_FULLSCREEN;

    if(SDL_SetVideoMode(settings_.resolution.x, settings_.resolution.y, Manager::COLOR_DEPTH, flags) == nullptr)
        return false;
      
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return false;
    }

    // We want the following properties to our display:
    //   (0;0) is the top-left corner of the screen
    //   (w;h) is the bottom-right corner of the screen
    // Since by default, (0;0) is the center of the screen, with (-1;1) being 
    // the top-left and (1;-1) the bottom-right, we must do the following:
    //   - Offset it by (-1,1), correcting the origin
    //   - Invert the y-axis, so it grows in the direction we expect
    //   - Scale down by the (2/w;2/h), so it goes up to what we expect.
    initial_geometry_ = Geometry(math::Vector2D(-1.0, 1.0), math::Vector2D(2.0/settings_.resolution.x, -2.0/settings_.resolution.y));
        
    //Set projection
    glViewport(0, 0, settings_.resolution.x, settings_.resolution.y);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //glOrtho( 0, size.x, size.y, 0, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // This hint can improve the speed of texturing when perspective- correct texture coordinate interpolation isn't needed, such as when using a glOrtho() projection.
    // From http://www.mesa3d.org/brianp/sig97/perfopt.htm
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //If there was any errors
    if( glGetError() != GL_NO_ERROR )
        return false;

    // Changing to and from fullscreen destroys all textures, so we must recreate them.
    initializeLight();

    shaders_.ReplaceShader(0, CreateShader(false, false));
    shaders_.ReplaceShader(1, CreateShader( true, false));
    shaders_.ReplaceShader(2, CreateShader(false,  true));
    shaders_.ReplaceShader(3, CreateShader( true,  true));

    light_shader_ = LightShader();
    return true;
}

void Manager::Release() {
    /*if(GLEW_ARB_framebuffer_object) {
        glDeleteFramebuffersEXT(1, &light_buffer_id_);
    }*/
}

void Manager::UpdateVSync() {
#ifdef _WIN32
    if(WGL_EXT_swap_control)
        wglSwapIntervalEXT(settings_.vsync ? 1 : 0); // sets VSync to "ON".
#else
    //TODO: IMPLEMENT in Linux. Refer to http://www.opengl.org/wiki/Swap_Interval for instructions.
#endif
}

void Manager::mergeLights(const std::list<action::Scene*>& scene_list) {
    // Lights are simply added together.
    glBlendFunc(GL_ONE, GL_ONE);

    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_PIXEL_MODE_BIT); // for GL_DRAW_BUFFER and GL_READ_BUFFER
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);

    for(std::list<action::Scene*>::const_iterator it = scene_list.begin(); it != scene_list.end(); ++it)
        if (!(*it)->finished())
            (*it)->content_node()->RenderLight(initial_geometry_, VisualEffect());
    
    // copy the framebuffer pixels to a texture
    glBindTexture(GL_TEXTURE_2D, light_buffer_->gltexture());
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, settings_.resolution.x, settings_.resolution.y);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopAttrib(); // GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT

    // Clear the screen so it's back to how it was before.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

// Desenha backbuffer na tela
void Manager::Render(const std::list<action::Scene*>& scene_list) {

    ///==================================================
    // Draw all lights to a buffer, merging then to a light texture.
    if(settings_.light_system) {
        mergeLights(scene_list);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    // Change the shader to the LightSystem shader, and bind the light texture.
    if(settings_.light_system) {
        shaders_.ChangeFlag(Shaders::USE_LIGHT_BUFFER, true);
        opengl::ShaderProgram::Use shader_use(shaders_.current_shader());
        shader_use.SendTexture(1, light_buffer_, shaders_.current_shader()->UniformLocation("light_texture"));
    }
    ///==================================================

    ///==================================================
    // Draw all the sprites from all scenes.
    for(std::list<action::Scene*>::const_iterator it = scene_list.begin(); it != scene_list.end(); ++it)
        if (!(*it)->finished())
            (*it)->content_node()->Render(initial_geometry_, VisualEffect());
    ///==================================================


    ///==================================================
    if(settings_.light_system) {
        shaders_.ChangeFlag(Shaders::USE_LIGHT_BUFFER, false);
    }
    // Draw all interface layers, with the usual RGBA blend.
    for(std::list<action::Scene*>::const_iterator it = scene_list.begin(); it != scene_list.end(); ++it)
        if (!(*it)->finished())
            (*it)->interface_node()->Render(initial_geometry_, VisualEffect());
    ///==================================================
    
    for(action::Scene* it : scene_list)
        it->Render(initial_geometry_, VisualEffect());

    // Swap the buffers to show the backbuffer to the user.
    SDL_GL_SwapBuffers();
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
