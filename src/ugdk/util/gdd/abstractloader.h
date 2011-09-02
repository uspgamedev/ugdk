
#ifndef UGDK_UTIL_GDD_ABSTRACTLOADER_H_
#define UGDK_UTIL_GDD_ABSTRACTLOADER_H_

namespace ugdk {

namespace gdd {

template <class T>
class LoaderProtocol;

template <class T>
class AbstractLoader {

  public:

    virtual ~AbstractLoader() {}

    virtual T* Load(std::string gddfile_path) = 0;

  protected:

    LoaderProtocol<T> *protocol_;

    virtual void newData(T* new_data) = 0;

    virtual T*& data() = 0;

    virtual void cleanData() = 0;

    AbstractLoader(LoaderProtocol<T> *protocol) : protocol_(protocol) {}

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_GDD_ABSTRACTLOADER_H_ */
