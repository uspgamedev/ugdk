#include <ugdk/text/language.h>

#include <ugdk/resource/module.h>
#include <ugdk/text/module.h>
#include <ugdk/text/label.h>
#include <ugdk/text/textbox.h>
#include <ugdk/filesystem/module.h>
#include <ugdk/filesystem/file.h>
#include <ugdk/text/languageword.h>
#include <ugdk/util/utf8.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <algorithm>

namespace ugdk {
namespace text {

using std::string;

#define STRING_LENGTH 1024

std::string LoadTextFromFile(const std::string& path) {
    auto file = ugdk::filesystem::manager()->OpenFile(path);
    if (!file)
        return std::string();

    static const int MAXLINE = 1024;

    char buffer_utf8[MAXLINE];
    // Read from the UTF-8 encoded file.

    std::string output;
    while(file->fgets(buffer_utf8, MAXLINE) != nullptr) {
#ifdef _WIN32
        size_t len = strlen(buffer_utf8);
        if (len > 1 && buffer_utf8[len - 2] == '\r') {
            buffer_utf8[len - 2] = buffer_utf8[len - 1];
            buffer_utf8[len - 1] = buffer_utf8[len];
        }
#endif
        output.append(buffer_utf8);
    }
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

    text::manager()->AddFont(name, path, font_size);
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
        // Store the UTF-8 'start + 1'.
        std::string decoded_text(start + 1);
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
    if(strcmp(str, "#WORDS") == 0)
        return TITLE_WORDS;
    if(strcmp(str, "#FILES") == 0)
        return TITLE_FILES;
    if(strcmp(str, "#FONTS") == 0)
        return TITLE_FONTS;
    return TITLE_UNKNOWN;
}

void trim_trailing_whitespace(char *str) {
    if (*str == 0)  // All spaces?
        return;

    // Trim trailing space
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;

    // Write new null terminator
    *(end + 1) = 0;
}

// Fills the map with the information on the given file
bool Language::Load(const std::string& language_file) {
    auto file = filesystem::manager()->OpenFile(language_file);
    if(!file)
        return false;

    char buffer_raw[STRING_LENGTH];
    int reading_type = 0;

    while (file->fgets(buffer_raw, STRING_LENGTH)) {
        // Read from the UTF-8 encoded file.
        trim_trailing_whitespace(buffer_raw);
   
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
            resource::manager()->GetContainer<LanguageWord*>()->Replace(result.second, result.first);

        } else {
            // Syntax error!
        }
    }
    return true;
}

} // namespace text
} // namespace ugdk
