#ifndef UGDK_GRAPHIC_EXCEPTIONS_H_
#define UGDK_GRAPHIC_EXCEPTIONS_H_

#include <ugdk/system/exceptions.h>

namespace ugdk {
namespace graphic {

class NotSupportedException : public system::BaseException {
  public:
    NotSupportedException(const char* reason) : system::BaseException(reason) {}
};

} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_EXCEPTIONS_H_