#ifndef UGDK_SYSTEM_EXCEPTIONS_H_
#define UGDK_SYSTEM_EXCEPTIONS_H_

#include <string>

namespace ugdk {
namespace system {

class BaseException {
  public:
    template <typename... Args>
    BaseException(const std::string& fmt, Args... args) {
        set_reason(fmt.c_str(), args...);
    }
    virtual ~BaseException() throw() {}

    inline virtual const char * what() const throw() {
        return reason_.c_str();
    }

  private:
    std::string reason_;
    void set_reason(const char* fmt, ...);
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
