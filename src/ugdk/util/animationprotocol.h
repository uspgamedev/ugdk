
#ifndef UGDK_UTIL_ANIMATIONPROTOCOL_H_
#define UGDK_UTIL_ANIMATIONPROTOCOL_H_

#include <ugdk/action/animationset.h>
#include <ugdk/util/gdd/descriptionprotocol.h>

namespace ugdk {

class AnimationProtocol : public gdd::DescriptionProtocol<AnimationSet> {

  public:

    AnimationProtocol() : current_animation_(NULL) {}
    virtual ~AnimationProtocol() {}

    bool NewDescription();

    bool NewData(const gdd::GDDString& data_name);

    bool NewProperty(const gdd::GDDString& property_name, const gdd::GDDArgs& property_args);

    bool NewRing(const gdd::GDDString& ring_typename);

    bool NewEntry(const gdd::GDDString& entry_name, const gdd::GDDArgs& entry_args);

    bool NewSimpleChain(const gdd::GDDString& ring_typename, const gdd::GDDArgs& ring_args);

  private:

    AnimationManager::Animation *current_animation_;

};

} /* namespace ugdk */
#endif /* UGDK_UTIL_ANIMATIONPROTOCOL_H_ */
