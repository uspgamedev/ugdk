#include <ugdk/graphic/text/textmanager.h>

#include <ugdk/system/engine.h>
#include <ugdk/desktop/module.h>
#include <ugdk/desktop/window.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/text/label.h>
#include <ugdk/graphic/text/textbox.h>
#include <ugdk/graphic/text/font.h>
#include <ugdk/util/utf8.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <cwchar>

//Tamanho máximo de uma linha do arquivo de entrada
#define MAXLINE 1024

namespace ugdk {
namespace graphic {

using std::string;
using std::vector;
using std::map;

bool TextManager::Initialize() {
    setlocale (LC_ALL,"en_US.C-UTF-8");
    return true;
}

TextManager::~TextManager() {}

bool TextManager::Release() {
    for(map<string, Font*>::iterator font_it = fonts_.begin(); font_it != fonts_.end(); ++font_it)
        delete font_it->second;
    fonts_.clear();
    
    return true;
}

TextBox* TextManager::GetText(const std::string& text, const std::string& fonttag, int width) {
    Font *font = fonttag.size() > 0 ? fonts_[fonttag] : current_font_;
    return new TextBox(text, (width == -1) ? desktop::manager()->primary_window()->size().x : width, font);
}

TextBox* TextManager::GetText(const std::string& text) {
    std::string blank_font;
    return GetText(text, blank_font);
}

TextBox* TextManager::GetTextFromFile(const std::string& path, const std::string& font, int width) {
    std::string fullpath = ugdk::system::ResolvePath(path);
    FILE *txtFile = fopen(fullpath.c_str(), "r");
    if(txtFile == nullptr) return nullptr;
    char buffer_utf8[MAXLINE];
    string output;
    // Read from the UTF-8 encoded file.
    while(fgets(buffer_utf8, MAXLINE, txtFile)!=nullptr){
        output.append(buffer_utf8);
    }
    fclose(txtFile);
    return GetText(output, font, width);
}

TextBox* TextManager::GetTextFromFile(const std::string& path) {
    std::string blank_font;
    return GetTextFromFile(path, blank_font);
}

void TextManager::AddFont(const std::string& name, const std::string& path, double size) {
    if(fonts_.count(name) > 0) {
        delete fonts_[name];
    #ifdef DEBUG
        if(fonts_[name])
            fprintf(stderr, "Replacing a font tag: \"%s\"\n", name.c_str());
    #endif
    } else {
    #ifdef DEBUG
        fprintf(stderr, "Loading new font tag: \"%s\"\n", name.c_str());
    #endif
    }
    fonts_[name] = current_font_ = new Font(path, size);
}

}  // namespace graphic
}  // namespace ugdk
