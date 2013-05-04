#include <ugdk/graphic/videomanager.h>

#include <cmath>
#include <cassert>

#include "GL/glew.h"
#ifdef _WIN32
#   include "GL/wglew.h"
#endif
#define NO_SDL_GLEXT
#include "SDL_opengl.h"

#include <ugdk/base/engine.h>
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

// Inicializa o gerenciador de video, definindo uma
// resolucao para o programa. Retorna true em caso de
// sucesso.
bool VideoManager::Initialize(const string& title, const ugdk::math::Vector2D& size, bool fullscreen, const string& icon) {
    title_ = title;
    
    // Set window title.
    SDL_WM_SetCaption(title.c_str(), (icon.length() > 0) ? icon.c_str() : NULL );
    
    if(icon.length() > 0)
        SDL_WM_SetIcon(SDL_LoadBMP(icon.c_str()), NULL);
       
    if(ChangeResolution(size, fullscreen) == false)
        if(ChangeResolution(default_resolution, false) == false) {
            /* TODO: insert error message here. */
            return false;
        }

    glClearColor( 0.0, 0.0, 0.0, 0.0 );

    /*if(GLEW_ARB_framebuffer_object) {
        glGenFramebuffersEXT(1, &light_buffer_id_);
    }*/
    return true;
}

// Changes the resolution to the requested value.
// Returns true on success.
bool VideoManager::ChangeResolution(const ugdk::math::Vector2D& size, bool fullscreen) {
    Uint32 flags = SDL_OPENGL;
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    if(fullscreen) flags |= SDL_FULLSCREEN;

    if(SDL_SetVideoMode(static_cast<int>(size.x), static_cast<int>(size.y), VideoManager::COLOR_DEPTH, flags) == NULL)
        return false;
      
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return false;
    }

    SetVSync(settings_.vsync);

    // We want the following properties to our display:
    //   (0;0) is the top-left corner of the screen
    //   (w;h) is the bottom-right corner of the screen
    // Since by default, (0;0) is the center of the screen, with (-1;1) being 
    // the top-left and (1;-1) the bottom-right, we must do the following:
    //   - Offset it by (-1,1), correcting the origin
    //   - Invert the y-axis, so it grows in the direction we expect
    //   - Scale down by the (2/w;2/h), so it goes up to what we expect.
    initial_geometry_ = Geometry(math::Vector2D(-1.0, 1.0), math::Vector2D(2.0/size.x, -2.0/size.y));
        
    //Set projection
    glViewport(0, 0, (GLsizei) size.x, (GLsizei) size.y);
    glMatrixMode( GL_PROJECTION );

    glLoadIdentity();
    //glOrtho( 0, size.x, size.y, 0, -1, 1 );

    //Initialize modelview matrix
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

    video_size_ = size;
    settings_.fullscreen = fullscreen;

    // Changing to and from fullscreen destroys all textures, so we must recreate them.
    initializeLight();

    bool shader_flags[Shaders::NUM_FLAGS] = { false, false };
    shaders_.ReplaceShader(shader_flags, CreateShader(shader_flags[0], shader_flags[1])); // false, false
    shader_flags[0] = true;
    shaders_.ReplaceShader(shader_flags, CreateShader(shader_flags[0], shader_flags[1])); // true, false
    shader_flags[1] = true;
    shaders_.ReplaceShader(shader_flags, CreateShader(shader_flags[0], shader_flags[1])); // true, true
    shader_flags[0] = false;
    shaders_.ReplaceShader(shader_flags, CreateShader(shader_flags[0], shader_flags[1])); // false, true

    light_shader_ = LightShader();
    return true;
}

// Termina o gerenciador de video, retornando true em
// caso de sucesso.
bool VideoManager::Release() {
    /*if(GLEW_ARB_framebuffer_object) {
        glDeleteFramebuffersEXT(1, &light_buffer_id_);
    }*/
    return true;
}

void VideoManager::SetVSync(const bool active) {
    settings_.vsync = active;
    //TODO:IMPLEMENT in Linux. Refer to http://www.opengl.org/wiki/Swap_Interval for instructions.
#ifdef _WIN32
    if(WGL_EXT_swap_control)
        wglSwapIntervalEXT(settings_.vsync ? 1 : 0); // sets VSync to "ON".
#endif
}

void VideoManager::mergeLights(const std::list<action::Scene*>& scene_list) {
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
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (GLsizei) video_size_.x, (GLsizei) video_size_.y);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopAttrib(); // GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT

    // Clear the screen so it's back to how it was before.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

// Desenha backbuffer na tela
void VideoManager::Render(const std::list<action::Scene*>& scene_list) {

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

    // Draw all the sprites from all scenes.
    for(std::list<action::Scene*>::const_iterator it = scene_list.begin(); it != scene_list.end(); ++it)
        if (!(*it)->finished())
            (*it)->content_node()->Render(initial_geometry_, VisualEffect());

    if(settings_.light_system) {
        shaders_.ChangeFlag(Shaders::USE_LIGHT_BUFFER, false);
    }
    // Draw all interface layers, with the usual RGBA blend.
    for(std::list<action::Scene*>::const_iterator it = scene_list.begin(); it != scene_list.end(); ++it)
        if (!(*it)->finished())
            (*it)->interface_node()->Render(initial_geometry_, VisualEffect());

    // Swap the buffers to show the backbuffer to the user.
    SDL_GL_SwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void VideoManager::initializeLight() {
    if(light_buffer_ != NULL) delete light_buffer_;
    light_buffer_ = Texture::CreateRawTexture(static_cast<int>(video_size_.x), static_cast<int>(video_size_.y));
    glBindTexture(GL_TEXTURE_2D, light_buffer_->gltexture());
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei) video_size_.x, 
        (GLsizei) video_size_.y, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
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

const opengl::ShaderProgram* VideoManager::Shaders::current_shader() const {
    return shaders_[current_flag_value_];
}

void VideoManager::Shaders::ChangeFlag(Flag flag, bool value) {
    int flag_bit = 1 << static_cast<int>(flag);
    if(value)
        current_flag_value_ |= flag_bit; // Set the bit for this flag.
    else
        current_flag_value_ &= ~flag_bit; // Unset the bit for this flag.
}

void VideoManager::Shaders::ReplaceShader(const bool flags[NUM_FLAGS], opengl::ShaderProgram* program) {
    int result_flag = 0;
    for(int i = 0; i < NUM_FLAGS; ++i)
        if(flags[i])
            result_flag += (1 << i);
    delete shaders_[result_flag];
    shaders_[result_flag] = program;
}

VideoManager::Shaders::Shaders() : current_flag_value_(0) {
    int max_flags = 1 << NUM_FLAGS;
    for(int i = 0; i < max_flags; ++i)
        shaders_[i] = NULL;
}
        
VideoManager::Shaders::~Shaders() {
    int max_flags = 1 << NUM_FLAGS;
    for(int i = 0; i < max_flags; ++i)
        delete shaders_[i];
}

}  // namespace graphic
}  // namespace ugdk
