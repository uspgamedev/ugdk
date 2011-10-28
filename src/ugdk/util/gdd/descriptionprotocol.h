

#ifndef UGDK_UTIL_DESCRIPTIONPROTOCOL_H_
#define UGDK_UTIL_DESCRIPTIONPROTOCOL_H_

#include <string>
#include <vector>
#include <ugdk/util/gdd/abstractloader.h>
#include <cstdio>

namespace ugdk {

namespace gdd {

typedef std::string               GDDString;
typedef std::vector<GDDString>    GDDArgs;

class LoadError {
  public:
    enum Type {
        TYPE_MISMATCH,
        INVALID_VALUE
    };
};

template <class T>
class DescriptionProtocol {

  public:

    virtual ~DescriptionProtocol() {}

    void set_loader(AbstractLoader<T> *loader) { loader_ = loader; }

    AbstractLoader<T>* loader() const { return loader_; }

    virtual bool NewDescription() = 0;

    virtual bool NewData(const GDDString& data_name) = 0;

    virtual bool NewProperty(const GDDString& property_name,
                             const GDDArgs& property_args) = 0;

    virtual bool NewRing(const GDDString& ring_typename) = 0;

    virtual bool NewEntry(const GDDString& entry_name, const GDDArgs& entry_args) = 0;

    virtual bool NewSimpleChain(const GDDString& ring_typename,
                                  const GDDArgs& ring_args) = 0;

  protected:

    DescriptionProtocol() : loader_(NULL) {}

    void error(LoadError::Type error_type, std::string &msg = "") {
        switch (error_type) {
          case LoadError::TYPE_MISMATCH:
            fprintf(stderr, "Load Error: type mismatch. %s\n", msg.c_str());
            break;
          case LoadError::INVALID_VALUE:
            fprintf(stderr, "Load Error: invalid value. %s\n", msg.c_str());
            break;
          default:
            fprintf(stderr, "Load Error: unknown. %s\n", msg.c_str()); // \comofas
            break;
        }
    }

  private:

    AbstractLoader<T>   *loader_;

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_DESCRIPTIONPROTOCOL_H_ */
