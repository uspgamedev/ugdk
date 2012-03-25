
#ifndef UGDK_UTIL_GDD_ABSTRACTLOADER_H_
#define UGDK_UTIL_GDD_ABSTRACTLOADER_H_

namespace ugdk {

namespace gdd {

template <class T>
class DescriptionProtocol;

template <class T>
class AbstractLoader {

  public:

    virtual ~AbstractLoader() { delete protocol_; }

    virtual T* Load(const std::string& gddfile_path) = 0;

    virtual void newData(T* new_data) = 0;

    virtual T*& data() = 0;

    virtual void cleanData() = 0;

  protected:

    DescriptionProtocol<T>* protocol() const { return protocol_; }

    AbstractLoader(DescriptionProtocol<T> *protocol) : protocol_(protocol) {
        protocol_->set_loader(this);
    }

  private:

    DescriptionProtocol<T> *protocol_;

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_GDD_ABSTRACTLOADER_H_ */
