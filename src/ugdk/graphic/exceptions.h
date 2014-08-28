#ifndef UGDK_GRAPHIC_EXCEPTIONS_H_
#define UGDK_GRAPHIC_EXCEPTIONS_H_

#include <string>

namespace ugdk {
namespace graphic {

class GraphicException {
  public:
    GraphicException(const char* reason) : reason_(reason) {}

    const char* what() const throw() {
        return reason_.c_str();
    }

  private:
    std::string reason_;
};

class InvalidOperation : public GraphicException {
  public:
    InvalidOperation(const char* reason) : GraphicException(reason) {}
};

class NotSupportedException : public GraphicException {
  public:
    NotSupportedException(const char* reason) : GraphicException(reason) {}
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