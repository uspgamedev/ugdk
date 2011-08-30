#include <SDL/SDL_opengl.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/util/pathmanager.h>
#include <cmath>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/image.h>

namespace ugdk {

// Inicializa o gerenciador de video, definindo uma
// resolucao para o programa. Retorna true em caso de
// sucesso.
bool VideoManager::Initialize(const string& title, const Vector2D& size,
                              bool fullscreen, const string& icon) {
	ChangeResolution(size, fullscreen);
	if(icon.length() > 0) {
		SDL_WM_SetIcon(SDL_LoadBMP(icon.c_str()), NULL);
	}
    SDL_WM_SetCaption(title.c_str(), NULL);
    title_ = title;
    
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    blank_image_ = new Image;
    blank_image_->set_frame_size(Vector2D(50.0f, 50.0f));
    blank_image_->set_color(Image::CreateColor(0.0f, 1.0f, 0.0f));
    return true;
}

// Changes the resolution to the requested value.
// Returns true on success.
bool VideoManager::ChangeResolution(const Vector2D& size, bool fullscreen) {
    Uint32 flags = SDL_OPENGL;
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    if(fullscreen)
        flags |= SDL_FULLSCREEN;
    if(SDL_SetVideoMode(static_cast<int>(size.x), static_cast<int>(size.y),
            VideoManager::COLOR_DEPTH, flags) == NULL)
        return false;

    //Set projection
	glViewport(0, 0, size.x, size.y);
    glMatrixMode( GL_PROJECTION );

    glLoadIdentity();
    glOrtho( 0, size.x, size.y, 0, -1, 1 );

    //Initialize modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //If there was any errors
    if( glGetError() != GL_NO_ERROR )
        return false;

    video_size_ = size;
    fullscreen_ = fullscreen;
    virtual_bounds_ = Frame(0, 0, video_size_.x, video_size_.y);

    // Changing to and from fullscreen destroys all textures, so we must recreate them.
	InitializeLight();
    return true;
}

// Termina o gerenciador de video, retornando true em
// caso de sucesso.
bool VideoManager::Release() {
    for(map<string,Image*>::iterator it = image_memory_.begin();
            it != image_memory_.end(); ++it) {
        Image* img = it->second;
        img->Destroy();
        delete img;
    }
    image_memory_.clear();
    return true;
}

void VideoManager::TranslateTo(Vector2D& offset) {
    glLoadIdentity();
	// Smaller values causes floating point errors and don't increase the image quality.
	glTranslatef(floor(offset.x), floor(offset.y), 0);
    this->virtual_bounds_ = Frame(-offset.x, -offset.y,
                                  -offset.x+video_size_.x,
                                  -offset.y+video_size_.y);
}

// Desenha backbuffer na tela
void VideoManager::Render() {
    //Update screen
    SDL_GL_SwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

// Carrega imagem de um arquivo, fazendo o
// gerenciamento de memoria. Retorna NULL
// em caso de falha.
Image* VideoManager::LoadImageFile(const string& filepath) {
    if(image_memory_.count(filepath) == 0) {
    	std::string fullpath = PATH_MANAGER()->ResolvePath(filepath);
		Image* img = new Image();
		if(img == NULL)
			return NULL;
		if(!img->LoadFromFile(fullpath)) {
			delete img;
			return NULL;
		}
        image_memory_[filepath] = img;
    }
    return image_memory_[filepath];
}

void VideoManager::set_light_draw_mode(LightType mode) {
    light_draw_mode_ = mode;
    switch(light_draw_mode_) {
    case LIGHT_SOURCE:
        glBlendFunc(GL_ONE, GL_ONE);
        break;
    case LIGHT_ILLUMINATED:
        glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
    default: // case LIGHT_IGNORE:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
    }
}

void VideoManager::InitializeLight() {
	light_size_ = Vector2D(40.0f, 40.0f);
	if(light_image_ != NULL) {
		delete light_image_;
	}
    light_image_ = new Image;
    light_image_->CreateFogTransparency(light_size_ * 4.0f, light_size_);
}

}  // namespace framework
