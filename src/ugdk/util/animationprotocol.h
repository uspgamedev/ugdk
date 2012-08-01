
#ifndef UGDK_UTIL_ANIMATIONPROTOCOL_H_
#define UGDK_UTIL_ANIMATIONPROTOCOL_H_

#include <map>
#include <utility>
#include <ugdk/action/animationset.h>
#include <ugdk/util/gdd/descriptionprotocol.h>
#include <ugdk/graphic/modifier.h>

namespace ugdk {

class AnimationProtocol : public gdd::DescriptionProtocol<action::AnimationSet> {
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

    bool NewEntry(const gdd::GDDString& entry_name, const gdd::GDDArgs& entry_args);

  private:
    action::Animation *current_animation_;
    graphic::Modifier* current_effect_;
    ParsingScope current_scope_;
    bool composing_;

    typedef bool (AnimationProtocol::*EntryFunction) (const gdd::GDDArgs&);


    std::map< std::pair<ParsingScope, gdd::GDDString>, EntryFunction > entry_functions_;
    
    bool NewProperty_Fps(double);
    bool NewProperty_Compose(void);
    
    bool NewRing_Effect(void);
    bool NewRing_Frame(void);

    bool NewEntry_EffectNumber(int); //TODO: implement this function.
    bool NewEntry_EffectAlpha(double);
    bool NewEntry_EffectColor(std::string);
    bool NewEntry_EffectPosition( const gdd::GDDArgs& );
    bool NewEntry_EffectMirror(   const gdd::GDDArgs& );
    bool NewEntry_EffectSize(     const gdd::GDDArgs& );
    bool NewEntry_EffectRotation(double);

    bool NewEntry_FrameNumber(int);
    bool NewEntry_FrameAlpha(double);
    bool NewEntry_FrameColor(std::string);
    bool NewEntry_FramePosition( const gdd::GDDArgs& );
    bool NewEntry_FrameMirror(   const gdd::GDDArgs& );
    bool NewEntry_FrameSize(     const gdd::GDDArgs& );
    bool NewEntry_FrameRotation(double);


    const bool arg_is_not_doubleing(const gdd::GDDString&) {
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
