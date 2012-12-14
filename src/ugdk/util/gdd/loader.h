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
        ParseStatus::Type status = parser.Parse(gddfile_path);
        T* result = protocol_->FinalizeDescription();
        if(status != ParseStatus::OK) {
            delete result;
            return nullptr;
        }
        return result;
    }

  private:
    DescriptionProtocol<T> *protocol_;

};

} /* namespace gdd */
} /* namespace ugdk */

#endif /* UGDK_UTIL_GDD_LOADER_TH_ */
