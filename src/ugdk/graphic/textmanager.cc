#include <ugdk/graphic/textmanager.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <cwchar>

#include <freetype-gl++/texture-font.hpp>

#include <ugdk/config/config.h>
#include <ugdk/base/engine.h>
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
    setlocale (LC_ALL,"en_US.C-UTF-8");
    atlas_ = new freetypeglxx::TextureAtlas(512, 512, 1);
    return true;
}

TextManager::~TextManager() {}

bool TextManager::Release() {
    map<std::string,freetypeglxx::TextureFont*>::iterator font_it;
    for(font_it = fonts_.begin(); font_it != fonts_.end(); ++font_it)
        delete font_it->second;
    fonts_.clear();
    
    delete atlas_;
    atlas_ = NULL;
    return true;
}

Text* TextManager::GetText(const std::wstring& text, const std::string& fonttag, int width) {
    wstring subString;
    vector<wstring> lines;
    freetypeglxx::TextureFont *font = fonttag.size() > 0 ? fonts_[fonttag] : current_font_;
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
            cur_width += font->GetGlyph(text[i])->width();
        }
    }
    if(cur_width > 0) {
        subString = text.substr(last_break, text.length());
        lines.push_back(subString);
    }
    return new Text(lines, font);
}

Text* TextManager::GetText(const std::wstring& text) {
    std::string blank_font;
    return GetText(text, blank_font);
}

Text* TextManager::GetTextFromFile(const std::string& path, const std::string& font, int width) {
    std::string fullpath = PATH_MANAGER()->ResolvePath(path);
    FILE *txtFile = fopen(fullpath.c_str(), "r");
    if(txtFile == NULL) return NULL;
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

Text* TextManager::GetTextFromFile(const std::string& path) {
    std::string blank_font;
    return GetTextFromFile(path, blank_font);
}



void TextManager::AddFont(const std::string& name, const std::string& path, int size, char ident, bool fancy) {
    if(fonts_.count(name) > 0) {
        delete fonts_[name];
    #ifdef DEBUG
        fprintf(stderr, "Replacing a font tag: \"%s\"\n", name.c_str());
    #endif
    } else {
    #ifdef DEBUG
        fprintf(stderr, "Loading new font tag: \"%s\"\n", name.c_str());
    #endif
    }
    std::string fullpath = PATH_MANAGER()->ResolvePath(path);

    fonts_[name] = current_font_ = new freetypeglxx::TextureFont(atlas_, fullpath, size);
}

}  // namespace graphic
}  // namespace ugdk
