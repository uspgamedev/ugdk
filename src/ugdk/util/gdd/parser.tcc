
#include <ugdk/util/gdd/parser.th>
#include <ugdk/util/gdd/parserutility.h>
#include <cstdio>
#include <cctype>
#include <string>
#include <vector>

namespace ugdk {

namespace gdd {

using std::string;
using std::vector;

#define ASSERT_CHUNK(status, parseChunk, reader) \
    ASSERT_PARSE(((status = parseChunk(reader)) == ParseStatus::OK), NO_MSG, status)

template <class T>
ParseStatus::Type Parser<T>::Parse(string gddfile_path) {
    FILE *file = fopen(gddfile_path.c_str(), "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\"\n", gddfile_path.c_str());
        return ParseStatus::FILE_NOT_FOUND;
    }
    Reader read(file);
    ASSERT_PARSE(loader()->NewDescription(), NO_MSG, ParseStatus::LOAD_ERROR);
    return doParse(read);
}

template <class T>
ParseStatus::Type Parser<T>::doParse(Reader &read) {
    ParseStatus::Type   status;
    int                 token = 0;
    while ((token = read.Next()) != EOF)
        switch (token) {
          case '#':
            read.SkipComment();
            break;
          case '$':
            ASSERT_CHUNK(status, parseDataName, read);
            break;
          case '%':
            ASSERT_CHUNK(status, parseSimpleSegment, read);
            break;
          case '@':
            ASSERT_CHUNK(status, parseProperty, read);
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


template <class T>
ParseStatus::Type Parser<T>::parseDataName(Reader &read) {
    GDDString data_name;
    ASSERT_PARSE(read.untilNext(), ERR_EMPTY_FIELD("data"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(data_name), NO_MSG, ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(loader()->NewData(data_name), NO_MSG, ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}

template <class T>
ParseStatus::Type Parser<T>::parseSimpleSegment(Reader &read) {
    GDDString   segment_type;
    GDDArgs     values;
    ASSERT_PARSE(read.untilNext(), ERR_EMPTY_FIELD("simple segment"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(segment_type), NO_MSG, ParseStatus::SYNTAX_ERROR);
    read.ValueSequence(values);
    ASSERT_PARSE(loader()->NewSimpleSegment(segment_type, values), NO_MSG, ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}

template <class T>
ParseStatus::Type Parser<T>::parseProperty(Reader &read) {
    GDDString   property_name;
    GDDArgs     values;
    ASSERT_PARSE(read.untilNext(), ERR_EMPTY_FIELD("property"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(property_name), NO_MSG, ParseStatus::SYNTAX_ERROR);
    read.ValueSequence(values);
    ASSERT_PARSE(loader()->NewProperty(property_name, values), NO_MSG, ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}

} /* namespace gdd */

} /* namespace ugdk */
