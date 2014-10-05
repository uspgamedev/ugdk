#ifndef UGDK_TEXT_LANGUAGE_H_
#define UGDK_TEXT_LANGUAGE_H_

#include <string>
#include <ugdk/util.h>

namespace ugdk {
namespace text {

class Language {
  public:
    Language() {}
    ~Language() {}

    bool Load(const std::string& language_file);
};

} // namespace text
} // namespace ugdk

#endif // UGDK_UTIL_LANGUAGE_H_
