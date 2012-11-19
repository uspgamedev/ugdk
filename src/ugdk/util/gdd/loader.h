#ifndef UGDK_UTIL_GDD_LOADER_H_
#define UGDK_UTIL_GDD_LOADER_H_

#include <string>
#include <ugdk/util/gdd/parser.h>
#include <ugdk/util/gdd/descriptionprotocol.h>

namespace ugdk {
namespace gdd {

template <class T>
class Loader {
  public:
    Loader(DescriptionProtocol<T> *protocol)
        : protocol_(protocol) {}

    ~Loader() {}

    T* Load(const std::string& gddfile_path) {
        Parser parser(protocol_);
        switch(parser.Parse(gddfile_path)) {
          case ParseStatus::OK: 
            break;
          case ParseStatus::FILE_NOT_FOUND:
            fputs("NOT FILE", stderr);
            break;
          case ParseStatus::SYNTAX_ERROR:
            fputs("NOT SYNTAX", stderr);
            break;
          case ParseStatus::LOAD_ERROR:
            fputs("NOT LOAD", stderr);
            break;
        }
        return protocol_->FinalizeDescription();
    }

  private:
    DescriptionProtocol<T> *protocol_;

};

} /* namespace gdd */
} /* namespace ugdk */

#endif /* UGDK_UTIL_GDD_LOADER_TH_ */
