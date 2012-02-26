#ifndef UGDK_UTIL_LANGUAGE_H_
#define UGDK_UTIL_LANGUAGE_H_

#include <string>
#include <ugdk/base/resourcecontainer.h>
#include <ugdk/util.h>

namespace ugdk {

class Language {
  public:
    Language() {}
    ~Language() {}

    bool Load(const std::string& language_file);
};

}  // namespace ugdk

#endif /* UGDK_UTIL_LANGUAGE_H_ */
