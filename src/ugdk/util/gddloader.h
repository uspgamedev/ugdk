

#ifndef UGDK_UTIL_DATALOADER_H_
#define UGDK_UTIL_DATALOADER_H_

#include <string>
#include <vector>

namespace ugdk {

class GDDLoader {

  public:

    ~GDDLoader();

    typedef enum {
        OK, TYPE_MISMATCH, INVALID_VALUE
    } SyntaxStatus;

    SyntaxStatus NewData(const std::string& data_name) = 0;

    SyntaxStatus NewProperty(const std::string& property_name, const std::string& property_args) = 0;

  protected:

    GDDLoader();

};

} /* namespace ugdk */
#endif /* DATALOADER_H_ */
