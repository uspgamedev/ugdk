#include <ugdk/util/gdd/parser.h>

#include <cstdio>
#include <cctype>
#include <string>
#include <vector>

#include <ugdk/util/gdd/parserutility.h>

namespace ugdk {
namespace gdd {

using std::string;
using std::vector;

#define ASSERT_CHUNK(status, parseChunk, reader) \
    ASSERT_PARSE(((status = parseChunk(reader)) == ParseStatus::OK), NO_MSG, status)

ParseStatus::Type Parser::Parse(const string& gddfile_path) {
    Reader read(gddfile_path);
    if (!read.Begin()) {
        fprintf(stderr, "Could not open file '%s'\n", read.file_path().c_str());
        return ParseStatus::FILE_NOT_FOUND;
    }
    ASSERT_PARSE(loader()->NewDescription(), NO_MSG, ParseStatus::LOAD_ERROR);
    return doParse(read);
}

ParseStatus::Type Parser::doParse(Reader &read) {
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
            ERR_LOCATION(read);
            fprintf(stderr, "Unknown token '%c'\n", token);
            return ParseStatus::SYNTAX_ERROR;
        }
    return ParseStatus::OK;
}

ParseStatus::Type Parser::parseDataName(Reader &read) {
    GDDString data_name;

    ASSERT_PARSE(read.UntilNextTag(), ERR_EMPTY_FIELD(read, "data"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(data_name), NO_MSG/*TODO*/, ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(loader()->NewData(data_name), NO_MSG, ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}

ParseStatus::Type Parser::parseSimpleChain(Reader &read) {
    GDDString   segment_type;
    GDDArgs     values;

    ASSERT_PARSE(read.UntilNextTag(), ERR_EMPTY_FIELD(read, "simple segment"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(segment_type), NO_MSG/*TODO*/, ParseStatus::SYNTAX_ERROR);
    read.ValueSequence(values);
    ASSERT_PARSE(loader()->NewSimpleChain(segment_type, values),
                 fprintf(stderr,
                         "%s:%u -> Could not load simple chain.\n",
                         read.file_path().c_str(),
                         (unsigned)(read.line())),
                 /*NO_MSG,*/
                 ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}

ParseStatus::Type Parser::parseProperty(Reader &read) {
    GDDString   property_name;
    GDDArgs     values;

    ASSERT_PARSE(read.UntilNextTag(), ERR_EMPTY_FIELD(read, "property"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(property_name), NO_MSG/*TODO*/, ParseStatus::SYNTAX_ERROR);
    read.ValueSequence(values);
    ASSERT_PARSE(loader()->NewProperty(property_name, values), NO_MSG, ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}

ParseStatus::Type Parser::parseEntry(Reader &read) {
    GDDString   entry_type;
    GDDArgs     values;

    ASSERT_PARSE(read.UntilNextTag(), ERR_EMPTY_FIELD(read, "entry"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(entry_type), NO_MSG/*TODO*/, ParseStatus::SYNTAX_ERROR);
    read.ValueSequence(values);
    ASSERT_PARSE(loader()->NewEntry(entry_type, values), NO_MSG, ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}

ParseStatus::Type Parser::parseRing(Reader &read) {
    GDDString   ring_type;

    ASSERT_PARSE(read.UntilNextTag(), ERR_EMPTY_FIELD(read, "ring"), ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(read.Name(ring_type), NO_MSG/*TODO*/, ParseStatus::SYNTAX_ERROR);
    ASSERT_PARSE(loader()->NewRing(ring_type), NO_MSG, ParseStatus::LOAD_ERROR);
    return ParseStatus::OK;
}



} /* namespace gdd */
} /* namespace ugdk */
