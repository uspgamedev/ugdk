
#ifndef UGDK_UTIL_UNCOPIABLE_H_
#define UGDK_UTIL_UNCOPIABLE_H_

namespace ugdk {
namespace util {

class Uncopyable {
  protected:
    Uncopyable() {}
    ~Uncopyable() {}
  private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

} /* namespace util */
} /* namespace ugdk */

#endif /* UGDK_UTIL_UNCOPIABLE_H_ */
