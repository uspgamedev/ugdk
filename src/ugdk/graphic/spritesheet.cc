#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_opengl.h"

#include <ugdk/graphic/spritesheet.h>

#include <ugdk/base/engine.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/videomanager.h>

#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>

namespace ugdk {
namespace graphic {

static SDL_Surface* createBaseSurface(Uint32 width, Uint32 height) {
    SDL_Surface* surface = nullptr;
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
    return surface;
}

static void extractPartOfSurface(SDL_Surface* source, SDL_Surface* target, int topleft_x, int topleft_y, int width, int height) {
    SDL_LockSurface(source);
    SDL_LockSurface(target);

    Uint32 *source_pixels = static_cast<Uint32*>(source->pixels);
    Uint32 *target_pixels = static_cast<Uint32*>(target->pixels);
    
    for(int y = 0; y < height; ++y)
        for(int x = 0; x < width; ++x) {
            Uint8 r, g, b, a;
            SDL_GetRGBA(source_pixels[(y + topleft_y) * source->w + (x + topleft_x)], source->format, &r, &g, &b, &a);
            target_pixels[y * width + x] = SDL_MapRGBA(target->format, r, g, b, a);
        }
    
    SDL_UnlockSurface(target);
    SDL_UnlockSurface(source);
}


struct PixelSurface {
    SDL_Surface* surface;
    
    PixelSurface(SDL_Surface* _surface) : surface(_surface) {}
    PixelSurface(const std::string& filepath) : surface(IMG_Load(filepath.c_str())) {}
    ~PixelSurface() { if(surface) SDL_FreeSurface(surface); }
};

SpritesheetData::SpritesheetData(const std::string& filename) {
    std::string filepath = PATH_MANAGER()->ResolvePath(filename);
    file_data_.push_back(new PixelSurface(filepath));
#ifdef DEBUG
    if(file_data_.back()->surface == nullptr)
        fprintf(stderr, "SpritesheetData - nullptr received when loading \"%s\"\n", filepath.c_str());
#endif
}

SpritesheetData::SpritesheetData(const std::list<std::string>& filenames) {
    std::list<std::string>::const_iterator it;
    for(it = filenames.begin(); it != filenames.end(); ++it) {
        std::string filepath = PATH_MANAGER()->ResolvePath(*it);
        file_data_.push_back(new PixelSurface(filepath));
#ifdef DEBUG
        if(file_data_.back()->surface == nullptr)
            fprintf(stderr, "SpritesheetData - nullptr received when loading \"%s\"\n", filepath.c_str());
#endif
    }
}

SpritesheetData::~SpritesheetData() {
    for(std::vector<PixelSurface*>::iterator it = file_data_.begin(); it != file_data_.end(); ++it)
        delete *it;

    for(std::list<SpritesheetFrame>::iterator it = frames_.begin();
        it != frames_.end(); ++it)
        delete it->surface;
}

void SpritesheetData::AddFrame(int topleft_x, int topleft_y, int width, int height, const ugdk::math::Vector2D& hotspot, size_t file) {
    if(file >= file_data_.size() || file_data_[file]->surface == nullptr) return;
    
    SDL_Surface* surface = createBaseSurface(width, height);
    extractPartOfSurface(file_data_[file]->surface, surface, topleft_x, topleft_y, width, height);

    frames_.push_back(SpritesheetFrame(new PixelSurface(surface), hotspot));
}

void SpritesheetData::FillWithFramesize(int width, int height, const ugdk::math::Vector2D& hotspot, size_t file) {
    if(file >= file_data_.size() || file_data_[file]->surface == nullptr) return;

    for(int y = 0; y + height <= file_data_[file]->surface->h; y += height) {
        for(int x = 0; x + width <= file_data_[file]->surface->w; x += width) {
            AddFrame(x, y, width, height, hotspot);
        }
    }
}
    
void SpritesheetData::FillWithFramesizeFromAllFiles(int width, int height, const ugdk::math::Vector2D& hotspot) {
    for(size_t i = 0; i < file_data_.size(); ++i)
        FillWithFramesize(width, height, hotspot, i);
}

Spritesheet::Spritesheet(const SpritesheetData& data) {
    const std::list<SpritesheetData::SpritesheetFrame>& frames = data.frames();

    lists_base_ = glGenLists(static_cast<GLsizei>(frames.size()));

    std::list<SpritesheetData::SpritesheetFrame>::const_iterator it;
    GLuint id;
    for(it = frames.begin(), id = 0; it != frames.end(); ++it, ++id) {
        Texture* texture = Texture::CreateFromSurface(it->surface->surface);
        createList(id, texture, it->hotspot);
        frames_.push_back(texture);
        frame_sizes_.push_back(ugdk::math::Vector2D(static_cast<double>(texture->width()), static_cast<double>(texture->height())));
    }
}

Spritesheet::~Spritesheet() {
    glDeleteLists(lists_base_, static_cast<GLsizei>(frames_.size()));

    // Clear the Textures
    for(std::vector<Texture*>::iterator it = frames_.begin();
        it != frames_.end(); ++it)
        delete *it;
}

const ugdk::math::Vector2D& Spritesheet::frame_size(size_t frame_number) const {
    static const ugdk::math::Vector2D invalid_size(0.0, 0.0);
    return frame_number < frame_sizes_.size() ? frame_sizes_[frame_number] : invalid_size;
}

void Spritesheet::createList(GLuint id, Texture* texture, const ugdk::math::Vector2D& hotspot) {
    if(texture == nullptr) return;
    glColor3f(1.0, 1.0, 1.0);

    ugdk::math::Vector2D origin, target(static_cast<double>(texture->width()), static_cast<double>(texture->height()));
    origin -= hotspot;
    target -= hotspot;

    // Start the list
    glNewList(lists_base_ + id, GL_COMPILE); {
        // Enable textures then attach our texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture->gltexture());

        glBegin( GL_QUADS ); //Start quad
            //Draw square
            glTexCoord2d(0.0, 0.0);
            glVertex2d(  origin.x, origin.y );

            glTexCoord2d(1.0, 0.0);
            glVertex2d(  target.x, origin.y );

            glTexCoord2d(1.0, 1.0);
            glVertex2d(  target.x, target.y );

            glTexCoord2d(0.0, 1.0);
            glVertex2d(  origin.x, target.y );
        glEnd();

    } glEndList();
}

void Spritesheet::Draw(int frame_number, const ugdk::math::Vector2D& hotspot) const {
    const Modifier& mod = VIDEO_MANAGER()->CurrentModifier();
    if(!mod.visible()) return;

    bool popmatrix = false;
    if(mod.mirror() != MIRROR_NONE || hotspot.NormOne() > 1.0e-6) {
        glPushMatrix();
        // TODO: combine the matrices

        // hotspot
        glTranslated(-hotspot.x, -hotspot.y, 0.0);

        // horizontal flip
        if(mod.mirror() & MIRROR_HFLIP) {
            glScalef(-1.0, 1.0, 1.0);
        }

        // vertical flip
        if(mod.mirror() & MIRROR_VFLIP) {
            glScalef(1.0, -1.0, 1.0);
        }
        popmatrix = true;
    }
    glColor4dv(mod.color().val);

    glCallList(lists_base_ + frame_number);

    if(popmatrix) glPopMatrix();
}

Spritesheet* CreateSpritesheetFromTag(const std::string& tag) {
    using script::VirtualObj;
    if(tag.size() == 0) return nullptr;

    VirtualObj data = SCRIPT_MANAGER()->LoadModule("spritesheets." + SCRIPT_MANAGER()->ConvertPathToDottedNotation(tag));
    if(!data) return nullptr;

    std::list<std::string> filenames;
    {
        VirtualObj::Vector files = data["file"].value<VirtualObj::Vector>();
        for(VirtualObj::Vector::iterator it = files.begin(); it != files.end(); ++it)
            filenames.push_back(it->value<std::string>());
    }

    SpritesheetData sprite_data(filenames);

    if(data["fill"]) {
        // TODO: make fill a vector of fills.
        VirtualObj::Vector fill = data["fill"].value<VirtualObj::Vector>();
        int width = fill[0].value<int>();
        int height = fill[1].value<int>();
        ugdk::math::Vector2D* hotspot = fill[2].value<ugdk::math::Vector2D*>();
        int file = (fill.size() >= 4) ? fill[4].value<int>() : 0;
        sprite_data.FillWithFramesize(width, height, *hotspot, file);
    }
    if(data["frames"]) {
        VirtualObj::Vector frames = data["frames"].value<VirtualObj::Vector>();
        for(VirtualObj::Vector::iterator it = frames.begin(); it != frames.end(); ++it) {
            VirtualObj::Vector frame = it->value<VirtualObj::Vector>();
            int top_left_x = frame[0].value<int>();
            int top_left_y = frame[1].value<int>();
            int width = frame[2].value<int>();
            int height = frame[3].value<int>();
            ugdk::math::Vector2D* hotspot = frame[4].value<ugdk::math::Vector2D*>();
            int file = (frame.size() >= 6) ? frame[5].value<int>() : 0;
            sprite_data.AddFrame(top_left_x, top_left_y, width, height, *hotspot, file);
        }
    }

    return new Spritesheet(sprite_data);
}

}  // namespace graphic
}  // namespace ugdk
