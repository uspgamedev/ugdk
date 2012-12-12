
#ifndef UGDK_UTIL_GDD_PARSER_H_
#define UGDK_UTIL_GDD_PARSER_H_

#include <ugdk/util/gdd/descriptionprotocolbase.h>
#include <ugdk/util/gdd/reader.h>
#include <map>

namespace ugdk {

namespace gdd {

typedef struct ParseStatus {
    enum Type {
        OK,
        FILE_NOT_FOUND,
        LOAD_ERROR,
        SYNTAX_ERROR
    };
  private:
    ParseStatus() {}
} ParseStatus;

/**
 * Parser for the Game Data Description (GDD) syntax.
 */
class Parser {

  public:

    Parser(DescriptionProtocolBase* loader) : loader_(loader) {}

    ParseStatus::Type Parse(const std::string& gddfile_path);

    virtual ~Parser() {}

  protected:

    DescriptionProtocolBase* loader() { return loader_; }

    ParseStatus::Type doParse(Reader &read);

    ParseStatus::Type parseDataName(Reader &read);

    ParseStatus::Type parseSimpleChain(Reader &read);

    ParseStatus::Type parseProperty(Reader &read);

    ParseStatus::Type parseEntry(Reader &read);

    ParseStatus::Type parseRing(Reader &read);

  private:

    DescriptionProtocolBase* loader_;

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_GDD_PARSER_TH_ */
