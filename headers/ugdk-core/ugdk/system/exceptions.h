#ifndef UGDK_SYSTEM_EXCEPTIONS_H_
#define UGDK_SYSTEM_EXCEPTIONS_H_

#include <string>

namespace ugdk {
namespace system {

class BaseException {
  public:
      BaseException(const char* reason) : reason_(reason) {}

    const char* what() const throw() {
        return reason_.c_str();
    }

  private:
    std::string reason_;
};

class InvalidOperation : public BaseException {
  public:
    InvalidOperation(const char* reason) : BaseException(reason) {}
};

template<class T, typename ...Args>
void AssertCondition(bool condition, Args... args) {
    if (!condition) {
        throw T(std::forward<Args>(args)...);
    }
}

} // namespace system
} // namespace ugdk

#endif // UGDK_SYSTEM_EXCEPTIONS_H_