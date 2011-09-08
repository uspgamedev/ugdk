
#include <ugdk/util/gdd/parser.th>
#include <ugdk/util/gdd/simpleloader.h>
#include <ugdk/util/gdd/cachedloader.h>
#include <cstdio>
#include <cctype>
#include <string>
#include <vector>

namespace ugdk {

namespace gdd {

using std::string;
using std::vector;

void skipComment(FILE *file) {
    int token = 0;
    while ((token = fgetc(file)) != '\n' && token != EOF);
}

void skipSpace(FILE *file) {
    int token = 0;
    while ((token = fgetc(file)) == ' ' || token == '\t');
    ungetc(token, file);
}

bool isReserved(int token) {
    switch (token) {
      case '#':
      case '$':
      case '%':
      case '@':
      case '+':
      case '[':
      case ']':
          return true;
      default:
          return false;
    }
}

bool next(FILE *file) {
    int token = 0;
    while ((token = fgetc(file)) == ' ' || token == '\t');
    ungetc(token, file);
    return !isReserved(token) && token != '\n' && token != EOF;
}

bool readName(FILE *file, string &name) {
    int token = 0;
    token = fgetc(file);
    if (!isalpha(token) && token != '_') {
        if (isReserved(token))
            fputs("Data name must not be empty.", stderr);
        else
            fputs("Data name must begin with an alphabetic character or '_'.", stderr);
        return false;
    }
    name.push_back(token);
    while (isalnum(token = fgetc(file)) || token == '_')
        name.push_back(token);
    ungetc(token, file);
    return true;
}

void readValue(FILE *file, string &value) {
    int token = 0;
    while (isalnum(token = fgetc(file)) || token == '_' || token == '.')
        value.push_back(token);
    ungetc(token, file);
}

void readValueSequence(FILE *file, vector<string>& values) {
    while (next(file)) {
        string value = "";
        readValue(file, value);
        if (value.length() == 0) break;
        values.push_back(value);
    }
}

template <class T>
ParseStatus::Type Parser<T>::parseDataName(FILE *file) {
    skipSpace(file);
    string data_name = "";
    if (!readName(file, data_name))
        return ParseStatus::SYNTAX_ERROR;
    if (loader()->NewData(data_name) != LoadStatus::LOAD_OK)
        return ParseStatus::LOAD_ERROR;
    return ParseStatus::OK;
}

template <class T>
ParseStatus::Type Parser<T>::parseSimpleSegment(FILE *file) {
    skipSpace(file);
    string segment_type = "";
    vector<string> values;
    if (!readName(file, segment_type))
        return ParseStatus::SYNTAX_ERROR;
    readValueSequence(file, values);
    if (loader()->NewSimpleSegment(segment_type, values) != LoadStatus::LOAD_OK)
        return ParseStatus::LOAD_ERROR;
    return ParseStatus::OK;
}

template <class T>
ParseStatus::Type Parser<T>::Parse(string gddfile_path) {
    FILE *file = fopen(gddfile_path.c_str(), "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\"\n", gddfile_path.c_str());
        return ParseStatus::FILE_NOT_FOUND;
    }
    int token = 0;
    while ((token = fgetc(file)) != EOF)
        switch (token) {
          case '#':
            skipComment(file);
            break;
          case '$':
            if (parseDataName(file) != ParseStatus::OK)
                return ParseStatus::SYNTAX_ERROR;
            break;
          case '%':
            if (parseSimpleSegment(file) != ParseStatus::OK)
                return ParseStatus::SYNTAX_ERROR;
            break;
          case ' ':
          case '\t':
          case '\n':
            break;
          default:
            fprintf(stderr, "Unknown token '%c'", token);
            return ParseStatus::SYNTAX_ERROR;
        }
    return ParseStatus::OK;
}

} /* namespace gdd */

} /* namespace ugdk */
