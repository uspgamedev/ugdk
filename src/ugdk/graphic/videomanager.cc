#include <ugdk/config/config.h>
#include "SDL_opengl.h"
#include "SDL_image.h"
#include <cmath>

#include <ugdk/graphic/videomanager.h>

#include <ugdk/base/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>
#include <ugdk/util/pathmanager.h>

// VSync
//TODO:IMPLEMENT in Linux. Refer to http://www.opengl.org/wiki/Swap_Interval for instructions. 
#ifdef WIN32
    // VSync
    #include <gl/GL.h>
    #include "wglext.h"
    typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
    PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = NULL;
#endif

static void InitializeExtensions() {
    static bool initialized = false;
    if(initialized) return;
    
    initialized = true;
#ifdef WIN32
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC) wglGetProcAddress( "wglSwapIntervalEXT" );
#endif
}


#define LN255 5.5412635451584261462455391880218

namespace ugdk {
namespace graphic {

static Vector2D default_resolution(800.0f, 600.0f);

// Inicializa o gerenciador de video, definindo uma
// resolucao para o programa. Retorna true em caso de
// sucesso.
bool VideoManager::Initialize(const string& title, const Vector2D& size, bool fullscreen, const string& icon) {
    modifiers_.empty();

    if(ChangeResolution(size, fullscreen) == false)
        ChangeResolution(default_resolution, false);

    if(icon.length() > 0)
        SDL_WM_SetIcon(SDL_LoadBMP(icon.c_str()), NULL);
        
    // Set window title.
    SDL_WM_SetCaption(title.c_str(), NULL);
    title_ = title;
    
    InitializeExtensions();

    /*GLenum err = glewInit();
    if (GLEW_OK != err) {
        // TODO: check errors with glew
    }*/
    
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    /*if(GLEW_ARB_framebuffer_object) {
        glGenFramebuffersEXT(1, &light_buffer_id_);
    }*/
    return true;
}

// Changes the resolution to the requested value.
// Returns true on success.
bool VideoManager::ChangeResolution(const Vector2D& size, bool fullscreen) {
    Uint32 flags = SDL_OPENGL;
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    if(fullscreen) flags |= SDL_FULLSCREEN;

    if(SDL_SetVideoMode(static_cast<int>(size.x), static_cast<int>(size.y), VideoManager::COLOR_DEPTH, flags) == NULL)
        return false;
      
    SetVSync(settings_.vsync);
        
    //Set projection
    glViewport(0, 0, (GLsizei) size.x, (GLsizei) size.y);
    glMatrixMode( GL_PROJECTION );

    glLoadIdentity();
    glOrtho( 0, size.x, size.y, 0, -1, 1 );

    //Initialize modelview matrix
    glMatrixMode( GL_MODELVIEW );
    ClearModiferStack();
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
    virtual_bounds_ = Frame(0, 0, video_size_.x, video_size_.y);

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
#ifdef WIN32
    if(wglSwapIntervalEXT != NULL) wglSwapIntervalEXT(settings_.vsync ? 1 : 0); // sets VSync to "ON".
#endif
}

void VideoManager::MergeLights(std::list<Scene*>& scene_list) {
    // Lights are simply added together.
    glBlendFunc(GL_ONE, GL_ONE);

    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_PIXEL_MODE_BIT); // for GL_DRAW_BUFFER and GL_READ_BUFFER
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);

    for(std::list<Scene*>::iterator it = scene_list.begin(); it != scene_list.end(); ++it)
        if (!(*it)->finished())
           (*it)->root_node()->RenderLight();

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
        glTexCoord2f(         0.0f,          1.0f );
        glVertex2f(           0.0f,          0.0f );

        glTexCoord2f(         1.0f,          1.0f );
        glVertex2f(  video_size_.x,          0.0f );

        glTexCoord2f(         1.0f,          0.0f );
        glVertex2f(  video_size_.x, video_size_.y );

        glTexCoord2f(         0.0f,          0.0f );
        glVertex2f(           0.0f, video_size_.y );
    glEnd();

    glPopMatrix();
}

// Desenha backbuffer na tela
void VideoManager::Render(std::list<Scene*>& scene_list, std::list<Node*>& interface_list, float dt) {

    // Draw all lights to a buffer, merging then to a light texture.
    if(settings_.light_system)
        MergeLights(scene_list);

    // Usual blend function for drawing RGBA images.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw all the sprites from all scenes.
    for(std::list<Scene*>::iterator it = scene_list.begin(); it != scene_list.end(); ++it)
        if (!(*it)->finished())
            (*it)->root_node()->Render(dt);

    // Using the light texture, merge it into the screen.
    if(settings_.light_system)
        BlendLightIntoBuffer();

    // Draw all interface layers, with the usual RGBA blend.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (std::list<Node*>::iterator it = interface_list.begin(); it != interface_list.end(); ++it)
        (*it)->Render(dt);


    // Swap the buffers to show the backbuffer to the user.
    SDL_GL_SwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

static SDL_Surface* CreateLightSurface(const Vector2D& size, const Vector2D& ellipse_coef) {
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
    SDL_Surface *screen = SDL_GetVideoSurface();

    SDL_Surface *temp = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, screen->format->BitsPerPixel,
                                 screen->format->Rmask, screen->format->Gmask,
                                 screen->format->Bmask, screen->format->Amask);
    if(temp == NULL)
        return false;
    SDL_Surface *data = SDL_DisplayFormatAlpha(temp);
    SDL_FreeSurface(temp);
    if(data == NULL)
        return false;

    Vector2D origin = size * 0.5f;

    // Locks the surface so we can manage the pixel data.
    SDL_LockSurface(data);
    Uint32 *pixels = static_cast<Uint32*>(data->pixels);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Uint8 alpha = 0;

            // Formulae to detect if the point is inside the ellipse.
            Vector2D dist = Vector2D(j + 0.0f, i + 0.0f) - origin;
            dist.x /= ellipse_coef.x;
            dist.y /= ellipse_coef.y;
            float distance = Vector2D::InnerProduct(dist, dist);
            if(distance <= 1)
                alpha = static_cast<Uint8>(SDL_ALPHA_OPAQUE * exp(-distance * LN255));
            pixels[i * width + j] = SDL_MapRGBA(data->format, alpha, alpha, alpha, alpha);
        }
    }
    SDL_UnlockSurface(data);
    return data;
}

void VideoManager::InitializeLight() {
    Vector2D light_size(32.0f, 32.0f);
    if(light_texture_ != NULL) delete light_texture_;

    SDL_Surface* light_surface = CreateLightSurface(light_size * 2.0f, light_size);
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

void VideoManager::PushAndApplyModifier(const Modifier* apply) {
    Modifier top = (modifiers_.empty()) ? Modifier::IDENTITY : modifiers_.top();

    if(apply->flags() & Modifier::HAS_COLOR) top.ComposeColor(apply);
    top.ComposeMirror(apply);
    top.ComposeVisible(apply);

    glPushMatrix();

    if(apply->flags() & Modifier::HAS_TRANSFORMATION) {
        // Calculates the translation
        float tx, ty;
        if(apply->flags() & Modifier::TRUNCATES_WHEN_APPLIED) {
            tx = std::floor(apply->offset().x);
            ty = std::floor(apply->offset().y);
        } else {
            tx = apply->offset().x;
            ty = apply->offset().y;
        }

        // Calculates the scale
        float sx = apply->scale().x, sy = apply->scale().y;

        // Calculates the rotation
        float s = sin(apply->rotation()), c = cos(apply->rotation());

        // Builds the full transformation matrix all at once.
        float M[16] = {  sx*c, sy*s, 0.0f, 0.0f, // First column
                        -sx*s, sy*c, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f,
                           tx,   ty, 0.0f, 1.0f };

        //glTranslatef(apply->offset().x, apply->offset().y, 0.0f);
        //glScalef(apply->scale().x, apply->scale().y, 0.0f);
        //glRotatef(apply->rotation() * 57.2957795f, 0.0f, 0.0f, 1.0f);

        glMultMatrixf(M);
    }

    modifiers_.push(top);
}

bool VideoManager::PopModifier() {
    if(modifiers_.empty()) return false;
    modifiers_.pop();
    glPopMatrix();
    return true;
}

void VideoManager::ClearModiferStack() {
    while(PopModifier());
}

}  // namespace graphic
}  // namespace ugdk
