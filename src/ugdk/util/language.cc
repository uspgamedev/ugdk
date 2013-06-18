#include <cstdio>
#include <cstring>
#include <utility>
#include <algorithm>
#include "language.h"
#include <ugdk/graphic/textmanager.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/drawable/label.h>
#include <ugdk/graphic/drawable/textbox.h>
#include <ugdk/system/engine.h>
#include <ugdk/util/languageword.h>
#include <ugdk/util/utf8.h>

namespace ugdk {

using std::map;
using std::string;
using std::wstring;

#define STRING_LENGTH 1024

std::wstring LoadTextFromFile(const std::string& path) {
    std::wstring output;
    std::string fullpath = ugdk::system::ResolvePath(path);

    FILE *txtFile = fopen(fullpath.c_str(), "r");
    if(txtFile == nullptr) return output;

    static const int MAXLINE = 1024;

    char buffer_utf8[MAXLINE];
    wchar_t buffer[MAXLINE];
    // Read from the UTF-8 encoded file.
    while(fgets(buffer_utf8, MAXLINE, txtFile) != nullptr){
        // Converting UTF-8 to wstring
        size_t buffer_size = utf8_to_wchar(buffer_utf8, strlen(buffer_utf8), buffer, MAXLINE, 0);
        buffer[buffer_size] = L'\0';
        output.append(buffer);
    }
    fclose(txtFile);
    return output;
}

//===================================================================
//  FONT
static std::string font_def = "[]:";

static bool IsFont(char *str) {
    std::string s(str);
    size_t search = 0;
    for(std::string::const_iterator it = font_def.begin(); it != font_def.end(); ++it)
        search = s.find_first_of(*it, search);
    return search != std::string::npos;
}

static void ReadFont(char* str) {
    std::string s(str);

    size_t start_name = s.find_first_of(font_def[0]) + 1;
    size_t   end_name = s.find_first_of(font_def[1], start_name);
    size_t start_path = s.find_first_of(font_def[2], end_name + 1) + 1;

    std::string name = s.substr(start_name, end_name - start_name);
    std::string size = s.substr(end_name + 1, start_path - end_name - 2);
    std::string path = s.substr(start_path);
    if(path[path.size()-1] == '\n')
        path.erase(path.size()-1);

    double font_size = atof(size.c_str());

    TEXT_MANAGER()->AddFont(name, path, font_size);
}

//===================================================================
//  WORD
static std::string key_def = "[]{}:\0";

// Returns true if the string matches the criteria of being a key
static bool IsWord(char* str) {
    if (str[0] != '[') return false;
    char *tmp = str;
    for(int i = 1; i<5/*key_def[i] != '\0'*/; ++i) {
        tmp = strchr(tmp, key_def[i]);
        if(tmp == nullptr) return false;
    }
    return true;
}

static std::pair<LanguageWord*, std::string> ReadWord(char* str, bool from_file) {
    char buffer[STRING_LENGTH];
    strcpy(buffer, str);

    // Name
    char *start = strchr(buffer, key_def[0]);
    char *end =   strchr(buffer, key_def[1]);
    end[0] = '\0';
    std::string name(start + 1);

    // Font
    start = strchr(end + 1, key_def[2]);
    end =   strchr(end + 1, key_def[3]);
    end[0] = '\0';
    std::string font(start + 1);

    // Value
    start = strchr(end + 1, key_def[4]);
    end =   strchr(end + 1, '\n');
    if(end != nullptr) end[0] = '\0';

    if(from_file) {
        std::string filepath(start + 1);
        return std::pair<LanguageWord*, std::string>(new LanguageWord(LoadTextFromFile(filepath), font), name);
    } else {
        // Decode the UTF-8 'start + 1' into widechars.
        wchar_t wide_buffer[STRING_LENGTH];
        size_t buffer_size = utf8_to_wchar(start + 1, strlen(start + 1), wide_buffer, STRING_LENGTH, 0);
        wide_buffer[buffer_size] = L'\0';

        std::wstring decoded_text(wide_buffer);
        return std::pair<LanguageWord*, std::string>(new LanguageWord(decoded_text, font), name);
    }
}

// Returns true if the first character in the string is a newline or \0
bool is_blank(char* str) {
    return (str[0] == '\n' || str[0] == '\0' || str[0] == '/');
}

// Returns true if the string matches the criteria of being a title
static bool is_title(char* str) {
    return (str[0] == '#');
}

// Returns the id of the title. See implementation of a list of IDs.
// Returns 0 if it's an unknown title.
#define TITLE_UNKNOWN 0
#define TITLE_WORDS   1
#define TITLE_FILES   2
#define TITLE_FONTS   3

static int title_type(char* str) {
    if(strcmp(str, "#WORDS\n") == 0)
        return TITLE_WORDS;
    if(strcmp(str, "#FILES\n") == 0)
        return TITLE_FILES;
    if(strcmp(str, "#FONTS\n") == 0)
        return TITLE_FONTS;
    return TITLE_UNKNOWN;
}

// Fills the map with the information on the given file
bool Language::Load(const std::string& language_file) {
    FILE* file = fopen(ugdk::system::ResolvePath(language_file).c_str(), "r");
    if(file == nullptr)
        return false;

    char buffer_raw[STRING_LENGTH];
    wchar_t buffer[STRING_LENGTH];
    int reading_type = 0;

    while(!feof(file)) {
        // Read from the UTF-8 encoded file.
        fgets(buffer_raw, STRING_LENGTH, file);

        // Converting UTF-8 to wstring
        size_t buffer_size = utf8_to_wchar(buffer_raw, strlen(buffer_raw), buffer, STRING_LENGTH, 0);
        buffer[buffer_size] = L'\0';

        if(is_blank(buffer_raw)) { // Is this line blank?
            continue;
        }

        if(is_title(buffer_raw)) { // check if this line is a title
            reading_type = title_type(buffer_raw);
        }

        if(reading_type == TITLE_FONTS) {
            if(IsFont(buffer_raw))
                ReadFont(buffer_raw);

        } else if(IsWord(buffer_raw)) {

            std::pair<LanguageWord*, std::string> result = ReadWord(buffer_raw, reading_type == TITLE_FILES);
            resource::manager()->get_container<LanguageWord*>()->Replace(result.second, result.first);

        } else {
            // Syntax error!
        }
    }
    fclose(file);
    return true;
}

} //  namespace ugdk
