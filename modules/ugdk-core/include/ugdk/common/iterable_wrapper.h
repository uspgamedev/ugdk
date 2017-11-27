
#ifndef UGDK_COMMON_ITERABLE_WRAPPER_H_
#define UGDK_COMMON_ITERABLE_WRAPPER_H_

namespace ugdk {
namespace common {

template <typename T>
class IterableWrapper {
  public:
    IterableWrapper(const T& iterable) : iterable_(iterable) {}

    typename T::const_iterator begin() const {
        return iterable_.begin();
    }

    typename T::const_iterator end() const {
        return iterable_.end();
    }

    typename T::const_iterator cbegin() {
        return iterable_.begin();
    }

    typename T::const_iterator cend() {
        return iterable_.end();
    }

    typename T::iterator begin() {
        return iterable_.begin();
    }

    typename T::iterator end() {
        return iterable_.end();
    }
  private:
    const T &iterable_;
};

template <typename T>
IterableWrapper MakeIterableWrapper(const T& iterable) {
    return IterableWrapper<T>(iterable);
}

} // namespace common
} // namespace ugdk

#endif // UGDK_COMMON_ITERABLE_WRAPPER_H_
