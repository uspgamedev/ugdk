
#ifndef UGDK_UTIL_ANIMATIONPROTOCOL_H_
#define UGDK_UTIL_ANIMATIONPROTOCOL_H_

#include <ugdk/action/animationset.h>
#include <ugdk/util/gdd/descriptionprotocol.h>

namespace ugdk {

class AnimationProtocol : public gdd::DescriptionProtocol<AnimationSet> {

  public:

    AnimationProtocol() : current_seq_(NULL) {}
    virtual ~AnimationProtocol() {}

    bool NewDescription();

    bool NewData(const gdd::GDDString& data_name);

    bool NewProperty(const gdd::GDDString& property_name, const gdd::GDDArgs& property_args);

    bool NewSegment(const gdd::GDDString& segment_typename);

    bool NewEntry(const gdd::GDDString& entry_name, const gdd::GDDArgs& entry_args);

    bool NewSimpleSegment(const gdd::GDDString& segment_typename, const gdd::GDDArgs& segment_args);

  private:

    Animation::FrameSequence *current_seq_;

};

} /* namespace ugdk */
#endif /* UGDK_UTIL_ANIMATIONPROTOCOL_H_ */
