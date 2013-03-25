#include <ugdk/util/animationprotocol.h>

#include <string>
#include <algorithm>
#include <cassert>
#include <ugdk/action/spriteanimationframe.h>

#define DEFAULT_FRAME 0
#define DEG_TO_RAD_FACTOR 0.00872664626

namespace ugdk {

using std::string;
using std::pair;
using std::bind;
using std::function;
using namespace std::placeholders;

using gdd::GDDString;
using gdd::GDDArgs;
using gdd::LoadError;

AnimationProtocol::AnimationProtocol() 
    : current_description_(nullptr), current_animation_(nullptr), current_effect_(nullptr), composing_(false) {
    //TODO: Make this map static maybe?

    //ENTRY_MAP_BULK_ASSIGN(EFFECT_RING, NewEntry_EffectNumber  , "number"  , "n");
    /*ENTRY_MAP_BULK_ASSIGN(EFFECT_RING, NewEntry_EffectAlpha   , "alpha"   , "a");
    ENTRY_MAP_BULK_ASSIGN(EFFECT_RING, NewEntry_EffectColor   , "color"   , "c");
    ENTRY_MAP_BULK_ASSIGN(EFFECT_RING, NewEntry_EffectPosition, "position", "p");
    ENTRY_MAP_BULK_ASSIGN(EFFECT_RING, NewEntry_EffectMirror  , "mirror"  , "m");
    ENTRY_MAP_BULK_ASSIGN(EFFECT_RING, NewEntry_EffectSize    , "size"    , "s");
    ENTRY_MAP_BULK_ASSIGN(EFFECT_RING, NewEntry_EffectRotation, "rotation", "r");

    ENTRY_MAP_BULK_ASSIGN(FRAME_RING , NewEntry_FrameNumber   , "number"  , "n");
    ENTRY_MAP_BULK_ASSIGN(FRAME_RING , NewEntry_FrameAlpha    , "alpha"   , "a");
    ENTRY_MAP_BULK_ASSIGN(FRAME_RING , NewEntry_FrameColor    , "color"   , "c");
    ENTRY_MAP_BULK_ASSIGN(FRAME_RING , NewEntry_FramePosition , "position", "p");
    ENTRY_MAP_BULK_ASSIGN(FRAME_RING , NewEntry_FrameMirror   , "mirror"  , "m");
    ENTRY_MAP_BULK_ASSIGN(FRAME_RING , NewEntry_FrameSize     , "size"    , "s");
    ENTRY_MAP_BULK_ASSIGN(FRAME_RING , NewEntry_FrameRotation , "rotation", "r");*/

    this->set_simple_chain_ringname("frame");

    RegisterBind(gdd::PROPERTY, "fps", &AnimationProtocol::NewProperty_Fps, this);
    RegisterBind(gdd::PROPERTY, "compose", &AnimationProtocol::NewProperty_Compose, this);
    
    RegisterBind(gdd::RING, "effect", &AnimationProtocol::NewRing_Effect, this);
    RegisterBind(gdd::RING, "frame", &AnimationProtocol::NewRing_Frame, this);
    
    RegisterBind(gdd::ENTRY, "number",   &AnimationProtocol::NewEntry_Number  , this);
    RegisterBind(gdd::ENTRY, "alpha",    &AnimationProtocol::NewEntry_Alpha   , this);
    RegisterBind(gdd::ENTRY, "color",    &AnimationProtocol::NewEntry_Color   , this);
    RegisterBind(gdd::ENTRY, "position", &AnimationProtocol::NewEntry_Position, this);
    RegisterBind(gdd::ENTRY, "mirror",   &AnimationProtocol::NewEntry_Mirror  , this);
    RegisterBind(gdd::ENTRY, "size",     &AnimationProtocol::NewEntry_Size    , this);
    RegisterBind(gdd::ENTRY, "rotation", &AnimationProtocol::NewEntry_Rotation, this);
    
    RegisterBind(gdd::ENTRY, "n", &AnimationProtocol::NewEntry_Number  , this);
    RegisterBind(gdd::ENTRY, "a", &AnimationProtocol::NewEntry_Alpha   , this);
    RegisterBind(gdd::ENTRY, "c", &AnimationProtocol::NewEntry_Color   , this);
    RegisterBind(gdd::ENTRY, "p", &AnimationProtocol::NewEntry_Position, this);
    RegisterBind(gdd::ENTRY, "m", &AnimationProtocol::NewEntry_Mirror  , this);
    RegisterBind(gdd::ENTRY, "s", &AnimationProtocol::NewEntry_Size    , this);
    RegisterBind(gdd::ENTRY, "r", &AnimationProtocol::NewEntry_Rotation, this);
}

AnimationProtocol::~AnimationProtocol() {
    delete current_effect_;
}

bool AnimationProtocol::NewDescription() {
    if(current_description_ != nullptr) return false;
    current_description_ = new action::SpriteAnimationTable();
    return true;
}

bool AnimationProtocol::NewData(const GDDString& data_name) {
    current_description_->Add(data_name, current_animation_ = new action::SpriteAnimation);
    delete current_effect_;
    current_effect_ = new action::SpriteAnimationFrame(DEFAULT_FRAME);
    //TODO: Verificar integridade da current_animation_
    return true;
}
    
action::SpriteAnimationTable* AnimationProtocol::FinalizeDescription() {
    action::SpriteAnimationTable* result = current_description_;
    current_description_ = nullptr;
    return result;
}
    
bool AnimationProtocol::NewProperty_Fps(double fps) {
    if(fps < 0.0) {
        string msg = "FPS must be positive.";
        return error(LoadError::INVALID_VALUE, msg);
    }
    current_animation_->set_fps(fps);
    return true;
}
bool AnimationProtocol::NewProperty_Compose(void) {
    return composing_ = true;
}

bool AnimationProtocol::NewRing_Effect(void) {
    if(!composing_) {
        delete current_effect_;
        current_effect_ = new action::SpriteAnimationFrame(DEFAULT_FRAME);
    }
    current_scope_ = EFFECT_RING;
    return true;
}
bool AnimationProtocol::NewRing_Frame(void) {
    current_animation_->Add(new action::SpriteAnimationFrame(*current_effect_));
    current_scope_ = FRAME_RING;
    return true;
}

bool AnimationProtocol::NewEntry_Number(int frame) {
    if(current_scope_ == FRAME_RING) {
        action::SpriteAnimationFrame& cur_frame =
            const_cast<action::SpriteAnimationFrame&>(current_animation_->At(current_animation_->size() - 1));
        cur_frame.set_spritesheet_frame(frame);
        return true;
    } else {
        /* TODO: implement the else */
        return false;
    }
}

bool AnimationProtocol::NewEntry_Alpha(double new_alpha) {
    new_alpha = std::min( std::max(new_alpha,0.0), 1.0 ); // new_alpha is of [0.0,1.0]

    if(current_scope_ == FRAME_RING) {
        action::SpriteAnimationFrame& cur_frame =
            const_cast<action::SpriteAnimationFrame&>(current_animation_->At(current_animation_->size() - 1));

        Color c = cur_frame.effect().color();
        if (composing_) c.a *= new_alpha;
        else            c.a  = new_alpha;
        cur_frame.effect().set_color(c);
    } else {
        Color c = current_effect_->effect().color();
        c.a = new_alpha;
        current_effect_->effect().set_color(c);
    }
    return true;
}
bool AnimationProtocol::NewEntry_Color(std::string arg) {
    if( arg_is_not_hexadecimal(arg) ) {
        string msg = "Invalid argument in an Entry of type Color,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    int new_color;
    sscanf(arg.c_str(), "%x", &new_color);
    int r = (new_color & 0xFF0000) >> 0x10  ,
        g = (new_color & 0x00FF00) >> 0x08  ,
        b =  new_color & 0x0000FF/*>> 0x00*/;

    Color c = Color(r/255.0, g/255.0, b/255.0);

    if(current_scope_ == FRAME_RING) {
        action::SpriteAnimationFrame& cur_frame =
            const_cast<action::SpriteAnimationFrame&>(current_animation_->At(current_animation_->size() - 1));
        if (composing_) cur_frame.effect().set_color(cur_frame.effect().color() * c);
        else            cur_frame.effect().set_color(c);
    } else {
        current_effect_->effect().set_color(c);
    }
    return true;
}
bool AnimationProtocol::NewEntry_Position(double x, double y) {
    ugdk::math::Vector2D new_pos(x, y);
    if(current_scope_ == FRAME_RING) {
        action::SpriteAnimationFrame& cur_frame =
            const_cast<action::SpriteAnimationFrame&>(current_animation_->At(current_animation_->size() - 1));
        if (composing_) cur_frame.geometry().ChangeOffset(cur_frame.geometry().offset() + new_pos);
        else            cur_frame.geometry().ChangeOffset(new_pos);
    } else
        current_effect_->geometry().ChangeOffset(new_pos);
    return true;

}
bool AnimationProtocol::NewEntry_Mirror(std::string arg) {
    std::transform(arg.begin(), arg.end(), arg.begin(), ::tolower);
    Mirror new_mirror = MIRROR_NONE;
    for(std::string::iterator it = arg.begin(); it != arg.end(); ++it) {
        char mirror_id = *it;
        switch(mirror_id) {
        case 'h': new_mirror |= MIRROR_HFLIP; break;
        case 'v': new_mirror |= MIRROR_VFLIP; break;
        default: return error(LoadError::INVALID_VALUE, "Invalid argument to Mirror");
        }
    }
    
    if(current_scope_ == FRAME_RING) {
        action::SpriteAnimationFrame& cur_frame =
            const_cast<action::SpriteAnimationFrame&>(current_animation_->At(current_animation_->size() - 1));
        if (composing_) cur_frame.set_mirror(cur_frame.mirror() | new_mirror);
        else            cur_frame.set_mirror(new_mirror);
    } else
        current_effect_->set_mirror(new_mirror);
    return true;
}
bool AnimationProtocol::NewEntry_Size(double x, double y) {
    ugdk::math::Vector2D new_size(x, y);
    if(current_scope_ == FRAME_RING) {
        action::SpriteAnimationFrame& cur_frame =
            const_cast<action::SpriteAnimationFrame&>(current_animation_->At(current_animation_->size() - 1));
        if (composing_) {
            cur_frame.geometry() *= graphic::Geometry(math::Vector2D(), new_size);
        } else {
            cur_frame.geometry() = graphic::Geometry(cur_frame.geometry().offset(), new_size, cur_frame.geometry().rotation());
        }
    } else {
        graphic::Geometry& geo = current_effect_->geometry();
        geo = graphic::Geometry(geo.offset(), new_size, geo.rotation());
    }
    return true;

}
bool AnimationProtocol::NewEntry_Rotation(double new_rot) {
    new_rot *= DEG_TO_RAD_FACTOR;
    if(current_scope_ == FRAME_RING) {
        action::SpriteAnimationFrame& cur_frame =
            const_cast<action::SpriteAnimationFrame&>(current_animation_->At(current_animation_->size() - 1));
        if (composing_) {
            cur_frame.geometry() *= graphic::Geometry(math::Vector2D(), math::Vector2D(1.0), new_rot);
        } else
            cur_frame.geometry() = graphic::Geometry(cur_frame.geometry().offset(), cur_frame.geometry().CalculateScale(), new_rot);
    } else {
        graphic::Geometry& geo = current_effect_->geometry();
        geo = graphic::Geometry(geo.offset(), geo.CalculateScale(), new_rot);
    }
    return true;
}
} /* namespace ugdk */
