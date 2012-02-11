#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <cwchar>
#include <ugdk/config/config.h>
#include "SDL_opengl.h"
#include <ugdk/base/engine.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/drawable/text.h>
#include <ugdk/graphic/font.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/util/utf8.h>

using namespace std;

//Tamanho máximo de uma linha do arquivo de entrada
#define MAXLINE 1024

namespace ugdk {
namespace graphic {

bool TextManager::Initialize() {
    TTF_Init();
    setlocale (LC_ALL,"en_US.C-UTF-8");
    return true;
}

TextManager::~TextManager() {
    TTF_Quit();
}

bool TextManager::Release() {
    map<wstring,Font*>::iterator font_it;
    for(font_it = fonts_.begin(); font_it != fonts_.end(); ++font_it)
        delete font_it->second;
    fonts_.clear();

    map<wstring,Texture**>::iterator imgs_it;
    for(imgs_it = font_images_.begin(); imgs_it != font_images_.end(); ++imgs_it) {
        for(int i = 0; i < 65535; i++)
            if(imgs_it->second[i] != NULL) {
                delete imgs_it->second[i];
            }
        delete[] imgs_it->second;
    }
    font_images_.clear();
    return true;
}

Text* TextManager::GetText(wstring text, wstring fonttag, int width) {
    wstring subString;
    vector<wstring> lines;
    Font *font = fonttag.size() > 0 ? fonts_[fonttag] : current_font_;
    int screensize = ((width == -1) ? static_cast<int>(VIDEO_MANAGER()->video_size().x) : width) - 200;
    int cur_width = 0, last_break = 0;
    for(unsigned int i = 0; i < text.length(); i++) {
        // Split text on newlines, or between words whenever the line is bigger than the screen.
        if(text[i] == L'\n' || (text[i] == L' ' && cur_width > screensize)) {
            subString = text.substr(last_break, i - last_break);
            lines.push_back(subString);
            last_break = i + 1;
            cur_width = 0;
        } else {
            cur_width += static_cast<int>(font->GetLetterSize(text[i]).x);
        }
    }
    if(cur_width > 0) {
        subString = text.substr(last_break, text.length());
        lines.push_back(subString);
    }
    return new Text(lines, font);
}
Text* TextManager::GetTextFromFile(wstring path, wstring font, int width) {
    std::string fullpath = PATH_MANAGER()->ResolvePath(path);
    FILE *txtFile = fopen(fullpath.c_str(), "r");
    if(txtFile==NULL) return NULL;
    char buffer_utf8[MAXLINE];
    wchar_t buffer[MAXLINE];
    wstring output;
    // Read from the UTF-8 encoded file.
    while(fgets(buffer_utf8, MAXLINE, txtFile)!=NULL){
        // Converting UTF-8 to wstring
        size_t buffer_size = utf8_to_wchar(buffer_utf8, strlen(buffer_utf8), buffer, MAXLINE, 0);
        buffer[buffer_size] = L'\0';
        output.append(buffer);
    }
    return GetText(output, font, width);
}

static Texture** LoadFontTexturesFromFile(const std::string& path) {
    Texture** font_image = new Texture*[65535];
    memset(font_image, 0, sizeof(*font_image)*65535);
    TTF_Font *ttf_font = TTF_OpenFont( path.c_str(), 100 );
    SDL_Color sdlcolor = { 255, 255, 255 };
    uint16 str[2];
    str[1] = '\0'; // End of string
    unsigned int i = 0x20;
    while(i < 256) {
        // For each possible character in the extended ASCII table, render and store it.
        // Could be improved to ignore non-renderable characters, like linefeed.
        str[0] = (uint16)(i);
        SDL_Surface *letter = TTF_RenderUNICODE_Blended( ttf_font, str, sdlcolor );
        font_image[i] = Texture::CreateFromSurface(letter);
        SDL_FreeSurface(letter);

        i++;
        if(i == 128)
            i = 160;
    }
    TTF_CloseFont( ttf_font );
    return font_image;
}

void TextManager::AddFont(wstring name, wstring path, int size, char ident, bool fancy) {
    if(fonts_.count(name) > 0) return;
#ifdef DEBUG
    fwprintf(stderr, L"Loading new font tag: \"%s\"\n", name.c_str());
#endif

    if(font_images_.count(path) == 0) // Given file not loaded, loading it.
        font_images_[path] = LoadFontTexturesFromFile(PATH_MANAGER()->ResolvePath(path));

    fonts_[name] = current_font_ = new Font(font_images_[path], size, ident, fancy);
}

}  // namespace graphic
}  // namespace ugdk
