#ifndef UGDK_GRAPHIC_EXCEPTIONS_H_
#define UGDK_GRAPHIC_EXCEPTIONS_H_

#include <string>

namespace ugdk {
namespace graphic {

class InvalidOperation {
  public:
    InvalidOperation(const char* reason) : reason_(reason) {}

    const char* what() const throw() {
        return reason_.c_str();
    }

  private:
    std::string reason_;
};

template<class T, typename ...Args>
void AssertCondition(bool condition, Args... args) {
    if (!condition) {
        throw T(std::forward<Args>(args)...);
    }
}

} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_EXCEPTIONS_H_