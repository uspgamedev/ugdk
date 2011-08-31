
#ifndef UGDK_UTIL_GDD_PARSER_H_
#define UGDK_UTIL_GDD_PARSER_H_

#include <ugdk/util/gdd/loaderinterface.h>
#include <map>

namespace ugdk {

namespace gdd {

/**
 * Parser for the Game Data Description (GDD) syntax.
 */
class Parser {

  public:

    typedef enum {
        OK, NOT_FOUND, SYNTAX_ERROR
    } ParseStatus;

    Parser() {}

    ParseStatus Parse(std::string gddfile_path, LoaderInterface* loader);

    virtual ~Parser() {}

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_GDD_PARSER_H_ */
