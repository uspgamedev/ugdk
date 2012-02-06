
#ifndef UGDK_UTIL_ANIMATIONPROTOCOL_H_
#define UGDK_UTIL_ANIMATIONPROTOCOL_H_

#include <map>
#include <utility>
#include <ugdk/action/animationset.h>
#include <ugdk/util/gdd/descriptionprotocol.h>
#include <ugdk/graphic/modifier.h>

namespace ugdk {

class AnimationProtocol : public gdd::DescriptionProtocol<AnimationSet> {

  public:

    enum ParsingScope {
        ANIMATION_DATA,
        FRAME_RING,
        EFFECT_RING
    };

    AnimationProtocol();
    virtual ~AnimationProtocol() { if(current_effect_) delete current_effect_; }

    bool NewDescription();

    bool NewData(const gdd::GDDString& data_name);

    bool NewProperty(const gdd::GDDString& property_name, const gdd::GDDArgs& property_args);

    bool NewRing(const gdd::GDDString& ring_typename);

    bool NewEntry(const gdd::GDDString& entry_name, const gdd::GDDArgs& entry_args);

    bool NewSimpleChain(const gdd::GDDString& ring_typename, const gdd::GDDArgs& ring_args);

  private:

    AnimationManager::Animation *current_animation_;

    graphic::Modifier* current_effect_;

    ParsingScope current_scope_;

    bool composing_;


    std::map< std::pair<ParsingScope, gdd::GDDString>, bool (AnimationProtocol::*) (const gdd::GDDArgs&) > entry_functions_;

    bool NewEntry_EffectNumber(   const gdd::GDDArgs& ); //TODO: implement this function.
    bool NewEntry_EffectAlpha(    const gdd::GDDArgs& );
    bool NewEntry_EffectColor(    const gdd::GDDArgs& );
    bool NewEntry_EffectPosition( const gdd::GDDArgs& );
    bool NewEntry_EffectMirror(   const gdd::GDDArgs& );
    bool NewEntry_EffectSize(     const gdd::GDDArgs& );
    bool NewEntry_EffectRotation( const gdd::GDDArgs& );

    bool NewEntry_FrameNumber(   const gdd::GDDArgs& );
    bool NewEntry_FrameAlpha(    const gdd::GDDArgs& );
    bool NewEntry_FrameColor(    const gdd::GDDArgs& );
    bool NewEntry_FramePosition( const gdd::GDDArgs& );
    bool NewEntry_FrameMirror(   const gdd::GDDArgs& );
    bool NewEntry_FrameSize(     const gdd::GDDArgs& );
    bool NewEntry_FrameRotation( const gdd::GDDArgs& );


    const bool arg_is_not_floating(const gdd::GDDString&) {
        //TODO: Implement this.
        return false;
    }
    const bool arg_is_not_integer(const gdd::GDDString&) {
        //TODO: Implement this.
        return false;
    }
    const bool arg_is_not_hexadecimal(const gdd::GDDString&) {
        //TODO: Implement this.
        return false;
    }
    const bool arg_is_not_flip_axis(const gdd::GDDString&) {
        //TODO: Implement this.
        return false;
    }

};

} /* namespace ugdk */
#endif /* UGDK_UTIL_ANIMATIONPROTOCOL_H_ */
