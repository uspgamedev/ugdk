/*
 * descriptionparser.cc
 *
 *  Created on: Aug 30, 2011
 *      Author: kazuo
 */

#include <ugdk/util/gdd/parser.h>

namespace ugdk {

namespace gdd {

using std::string;

Parser::ParseStatus Parser::Parse(string gddfile_path, BaseLoader* loader) {

    return ParseStatus::OK;
}

} /* namespace gdd */

} /* namespace ugdk */
