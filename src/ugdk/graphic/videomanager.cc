#include <ugdk/graphic/videomanager.h>

#include <cmath>

#include "GL/glew.h"
#ifdef _WIN32
#   include "GL/wglew.h"
#endif
#define NO_SDL_GLEXT
#include "SDL_opengl.h"

#include <ugdk/base/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/util/pathmanager.h>

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
        
    //Set projection
    glViewport(0, 0, (GLsizei) size.x, (GLsizei) size.y);
    glMatrixMode( GL_PROJECTION );

    glLoadIdentity();
    glOrtho( 0, size.x, size.y, 0, -1, 1 );

    //Initialize modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // This hint can improve the speed of texturing when perspective- correct texture coordinate interpolation isn't needed, such as when using a glOrtho() projection.
    // From http://www.mesa3d.org/brianp/sig97/perfopt.htm
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    glEnable(GL_BLEND);

    //If there was any errors
    if( glGetError() != GL_NO_ERROR )
        return false;

    video_size_ = size;
    settings_.fullscreen = fullscreen;
    virtual_bounds_ = math::Frame(0, 0, video_size_.x, video_size_.y);

    // Changing to and from fullscreen destroys all textures, so we must recreate them.
    InitializeLight();
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
            (*it)->content_node()->RenderLight(Geometry(), VisualEffect());

    // copy the framebuffer pixels to a texture
    glBindTexture(GL_TEXTURE_2D, light_buffer_->gltexture());
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (GLsizei) video_size_.x, (GLsizei) video_size_.y);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopAttrib(); // GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT

    // Clear the screen so it's back to how it was before.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void VideoManager::BlendLightIntoBuffer() {
    // BIND DA LIGHT TEXTURE. IT'S SO AWESOME
    glBindTexture(GL_TEXTURE_2D, light_buffer_->gltexture());

    glPushMatrix();
    glLoadIdentity();

    // TODO: check why the hell when using 
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Sometimes a light sets the entire scene to that color.

    glBlendFunc(GL_ZERO, GL_SRC_COLOR);

    glBegin( GL_QUADS );
        glTexCoord2d(         0.0,          1.0 );
        glVertex2d(           0.0,          0.0 );

        glTexCoord2d(         1.0,          1.0 );
        glVertex2d(  video_size_.x,          0.0 );

        glTexCoord2d(         1.0,          0.0 );
        glVertex2d(  video_size_.x, video_size_.y );

        glTexCoord2d(         0.0,          0.0 );
        glVertex2d(           0.0, video_size_.y );
    glEnd();

    glPopMatrix();
}

// Desenha backbuffer na tela
void VideoManager::Render(const std::list<action::Scene*>& scene_list) {

    // Draw all lights to a buffer, merging then to a light texture.
    if(settings_.light_system)
        mergeLights(scene_list);

    // Usual blend function for drawing RGBA images.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw all the sprites from all scenes.
    for(std::list<action::Scene*>::const_iterator it = scene_list.begin(); it != scene_list.end(); ++it)
        if (!(*it)->finished())
            (*it)->content_node()->Render(Geometry(), VisualEffect());

    // Using the light texture, merge it into the screen.
    if(settings_.light_system)
        BlendLightIntoBuffer();

    // Draw all interface layers, with the usual RGBA blend.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for(std::list<action::Scene*>::const_iterator it = scene_list.begin(); it != scene_list.end(); ++it)
        if (!(*it)->finished())
            (*it)->interface_node()->Render(Geometry(), VisualEffect());


    // Swap the buffers to show the backbuffer to the user.
    SDL_GL_SwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

static SDL_Surface* CreateLightSurface(const ugdk::math::Vector2D& size, const ugdk::math::Vector2D& ellipse_coef) {
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
    SDL_Surface *screen = SDL_GetVideoSurface();

    SDL_Surface *temp = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, screen->format->BitsPerPixel,
                                 screen->format->Rmask, screen->format->Gmask,
                                 screen->format->Bmask, screen->format->Amask);
    if(temp == NULL)
        return NULL;
    SDL_Surface *data = SDL_DisplayFormatAlpha(temp);
    SDL_FreeSurface(temp);
    if(data == NULL)
        return NULL;

    ugdk::math::Vector2D origin = size * 0.5;

    // Locks the surface so we can manage the pixel data.
    SDL_LockSurface(data);
    Uint32 *pixels = static_cast<Uint32*>(data->pixels);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Uint8 alpha = 0;

            // Formulae to detect if the point is inside the ellipse.
            ugdk::math::Vector2D dist = ugdk::math::Vector2D(j + 0.0, i + 0.0) - origin;
            dist.x /= ellipse_coef.x;
            dist.y /= ellipse_coef.y;
            double distance = ugdk::math::Vector2D::InnerProduct(dist, dist);
            if(distance <= 1)
                alpha = static_cast<Uint8>(SDL_ALPHA_OPAQUE * exp(-distance * LN255));
            pixels[i * width + j] = SDL_MapRGBA(data->format, alpha, alpha, alpha, alpha);
        }
    }
    SDL_UnlockSurface(data);
    return data;
}

void VideoManager::InitializeLight() {
    ugdk::math::Vector2D light_size(32.0, 32.0);
    if(light_texture_ != NULL) delete light_texture_;

    SDL_Surface* light_surface = CreateLightSurface(light_size * 2.0, light_size);
    light_texture_ = Texture::CreateFromSurface(light_surface);
    SDL_FreeSurface(light_surface);

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
}

}  // namespace graphic
}  // namespace ugdk
