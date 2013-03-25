#include <cstdio>
#include <cstring>
#include <utility>
#include "language.h"
#include <ugdk/graphic/textmanager.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/drawable/text.h>
#include <ugdk/base/engine.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/util/languageword.h>
#include <ugdk/util/utf8.h>

namespace ugdk {

using std::map;
using std::string;
using std::wstring;

#define STRING_LENGTH 1024

class WordString : public LanguageWord {
  public:
    WordString(const std::wstring& text, const std::string& font) : LanguageWord(font), text_(text) {}
    ugdk::graphic::Text* GenerateText() const {
        return TEXT_MANAGER()->GetText(text_, font_);
    }
  private:
    std::wstring text_;
};
    
class WordFile : public LanguageWord {
  public:
    WordFile(const std::string& filepath, const std::string& font) : LanguageWord(font), filepath_(filepath) {}
    ugdk::graphic::Text* GenerateText() const {
        return TEXT_MANAGER()->GetTextFromFile(filepath_, font_);
    }
  private:
    std::string filepath_;
};

//===================================================================
//  FONT
static std::string font_def = "[]{}:";

static bool IsFont(char *str) {
    if (str[0] != '[') return false;
    char* tmp = str;
    for(int i = 1; i<5/*font_def[i] != '\0'*/; ++i) {
        tmp = strchr(tmp, font_def[i]);
        if(tmp == nullptr) return false;
    }
    return true;
}

static void ReadFont(char* str) {
    char buffer[STRING_LENGTH];
    strcpy(buffer, str);

    // Get the tag name
    char *start = strchr(buffer, font_def[0]);
    char *end =   strchr(buffer, font_def[1]);
    end[0] = L'\0';
    std::string name = std::string(start + 1);
    end[0] = font_def[1];

    int font_size = 50;
    char ident = 'c';
    bool style = false;

    // Get the font size
    char* arg1 = strchr(buffer, '{');
    char* arg2 = strchr(arg1, '}');
    arg2[0] = L'\0';
    sscanf(arg1 + 1, "%d", &font_size);
    arg2[0] = L'}';

    if(arg2[1] != L':') {
        if(arg2[1] == L'+')
            style = true;
        else {
            ident = arg2[1];
            style = (arg2[2] == L'+');
        }
    }
    char *resp = strchr(buffer, ':');
           end = strchr(buffer + 1, '\n');
    if(end != nullptr) end[0] = '\0';

    TEXT_MANAGER()->AddFont(name, resp + 1, font_size, ident, style);
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
        return std::pair<LanguageWord*, std::string>(new WordFile(filepath, font), name);
    } else {
        // Decode the UTF-8 'start + 1' into widechars.
        wchar_t wide_buffer[STRING_LENGTH];
        size_t buffer_size = utf8_to_wchar(start + 1, strlen(start + 1), wide_buffer, STRING_LENGTH, 0);
        wide_buffer[buffer_size] = L'\0';

        std::wstring decoded_text(wide_buffer);
        return std::pair<LanguageWord*, std::string>(new WordString(decoded_text, font), name);
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
    FILE* file = fopen(PATH_MANAGER()->ResolvePath(language_file).c_str(), "r");
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
            RESOURCE_MANAGER()->word_container().Replace(result.second, result.first);

        } else {
            // Syntax error!
        }
    }
    fclose(file);
    return true;
}

} //  namespace ugdk
