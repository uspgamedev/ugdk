#ifndef UGDK_UTIL_ANIMATIONPROTOCOL_H_
#define UGDK_UTIL_ANIMATIONPROTOCOL_H_

#include <map>
#include <utility>
#include <ugdk/action.h>
#include <ugdk/action/spritetypes.h>
#include <ugdk/util/indexabletable.h>
#include <ugdk/util/gdd/descriptionprotocol.h>
#include <ugdk/graphic/modifier.h>

namespace ugdk {

class AnimationProtocol : public gdd::DescriptionProtocol< action::SpriteAnimationTable > {
  public:
    enum ParsingScope {
        ANIMATION_DATA,
        FRAME_RING,
        EFFECT_RING
    };

    AnimationProtocol();
    virtual ~AnimationProtocol();

    bool NewDescription();
    bool NewData(const gdd::GDDString& data_name);
    action::SpriteAnimationTable* FinalizeDescription();

  private:
    action::SpriteAnimationTable* current_description_;
    action::SpriteAnimation *current_animation_;
    action::SpriteAnimationFrame* current_effect_;
    ParsingScope current_scope_;
    bool composing_;

    bool NewProperty_Fps(double);
    bool NewProperty_Compose(void);
    
    bool NewRing_Effect(void);
    bool NewRing_Frame(void);

    bool NewEntry_Number(int); //TODO: implement this function.
    bool NewEntry_Alpha(double);
    bool NewEntry_Color(std::string);
    bool NewEntry_Position(double, double);
    bool NewEntry_Mirror(std::string);
    bool NewEntry_Size(double, double);
    bool NewEntry_Rotation(double);

    const bool arg_is_not_hexadecimal(const gdd::GDDString&) {
        //TODO: Implement this.
        return false;
    }
};

} /* namespace ugdk */
#endif /* UGDK_UTIL_ANIMATIONPROTOCOL_H_ */
