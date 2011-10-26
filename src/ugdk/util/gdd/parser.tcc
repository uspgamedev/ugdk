
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
    Reader read(gddfile_path);
    if (!read.Begin()) {
        fprintf(stderr, "Could not open file \"%s\"\n", gddfile_path.c_str());
        return ParseStatus::FILE_NOT_FOUND;
    }
    ASSERT_PARSE(loader()->NewDescription(), NO_MSG, ParseStatus::LOAD_ERROR);
    return doParse(read);
}

template <class T>
ParseStatus::Type Parser<T>::doParse(Reader &read) {
    ParseStatus::Type   status;
    int                 token = 0;
    while ((token = read.Next()) != EOF)
        switch (token) {
          case '#': // Comment
            read.SkipComment();
            break;
          case '$': // Data
            ASSERT_CHUNK(status, parseDataName, read);
            break;
          case '@': // Property
            ASSERT_CHUNK(status, parseProperty, read);
            break;
          case '+': // Ring
            ASSERT_CHUNK(status, parseRing, read);
            break;
          case '[': // Begin Entry
            ASSERT_CHUNK(status, parseEntry, read);
            break;
          case ']': // End Entry ; used for error handling.
            break;
          case '%': // Simple Chain ; is actually a syntax sugar for a "Subchain of Simple Rings".
            ASSERT_CHUNK(status, parseSimpleChain, read);
            break;
          //"skip whitespace" cases:
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

    ASSERT_PARSE(read.UntilNextTag(), ERR_EMPTY_FIELD("data"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(data_name), NO_MSG/*TODO*/, ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(loader()->NewData(data_name), NO_MSG, ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}

template <class T>
ParseStatus::Type Parser<T>::parseSimpleChain(Reader &read) {
    GDDString   segment_type;
    GDDArgs     values;

    ASSERT_PARSE(read.UntilNextTag(), ERR_EMPTY_FIELD("simple segment"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(segment_type), NO_MSG/*TODO*/, ParseStatus::SYNTAX_ERROR);
    read.ValueSequence(values);
    ASSERT_PARSE(loader()->NewSimpleChain(segment_type, values), NO_MSG, ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}

template <class T>
ParseStatus::Type Parser<T>::parseProperty(Reader &read) {
    GDDString   property_name;
    GDDArgs     values;

    ASSERT_PARSE(read.UntilNextTag(), ERR_EMPTY_FIELD("property"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(property_name), NO_MSG/*TODO*/, ParseStatus::SYNTAX_ERROR);
    read.ValueSequence(values);
    ASSERT_PARSE(loader()->NewProperty(property_name, values), NO_MSG, ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}

template <class T>
ParseStatus::Type Parser<T>::parseEntry(Reader &read) {
    GDDString   entry_type;
    GDDArgs     values;

    ASSERT_PARSE(read.UntilNextTag(), ERR_EMPTY_FIELD("entry"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(entry_type), NO_MSG/*TODO*/, ParseStatus::SYNTAX_ERROR);
    read.ValueSequence(values);
    ASSERT_PARSE(loader()->NewEntry(entry_type, values), NO_MSG, ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}

template <class T>
ParseStatus::Type Parser<T>::parseRing(Reader &read) {
    GDDString   ring_type;

    ASSERT_PARSE(read.UntilNextTag(), ERR_EMPTY_FIELD("ring"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(ring_type), NO_MSG/*TODO*/, ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(loader()->NewRing(ring_type), NO_MSG, ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}



} /* namespace gdd */

} /* namespace ugdk */
