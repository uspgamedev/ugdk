#include <ugdk/text/manager.h>

#include <ugdk/system/engine.h>
#include <ugdk/debug/log.h>
#include <ugdk/desktop/module.h>
#include <ugdk/desktop/window.h>
#include <ugdk/text/label.h>
#include <ugdk/text/textbox.h>
#include <ugdk/text/font.h>
#include <ugdk/util/utf8.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <cwchar>

namespace ugdk {
namespace text {

//Tamanho máximo de uma linha do arquivo de entrada
namespace {
    const int MAXLINE = 1024;
}

using std::string;
using std::vector;
using std::map;

Manager::Manager() : current_font_(nullptr) {}

Manager::~Manager() {}

bool Manager::Initialize() {
    setlocale(LC_ALL, "en_US.C-UTF-8");
    return true;
}

bool Manager::Release() {
    fonts_.clear();    
    return true;
}

TextBox* Manager::GetText(const std::string& text, const std::string& fonttag, int width) {
    Font *font = fonttag.size() > 0 ? fonts_[fonttag].get() : current_font_;
    return new TextBox(text, (width == -1) ? desktop::manager()->primary_window()->size().x : width, font);
}

TextBox* Manager::GetText(const std::string& text) {
    std::string blank_font;
    return GetText(text, blank_font);
}

TextBox* Manager::GetTextFromFile(const std::string& path, const std::string& font, int width) {
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

TextBox* Manager::GetTextFromFile(const std::string& path) {
    std::string blank_font;
    return GetTextFromFile(path, blank_font);
}

Font* Manager::GetFont(const std::string& name) const {
    auto it = fonts_.find(name);
    if (it != fonts_.end())
        return it->second.get();
    return nullptr;
}

void Manager::AddFont(const std::string& name, const std::string& path, double size) {
    debug::DebugConditionalLog(fonts_.find(name) != fonts_.end(), debug::LogLevel::INFO, "UGDK",
                               "Replacing a font tag: '", name, "'");
    debug::DebugConditionalLog(fonts_.find(name) == fonts_.end(), debug::LogLevel::INFO, "UGDK",
                               "Loading new font tag: '", name, "'");
    fonts_[name].reset(new Font(path, size));
    current_font_ = fonts_[name].get();
}

}  // namespace text
}  // namespace ugdk
