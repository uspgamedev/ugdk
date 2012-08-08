
#include <string>
#include <algorithm>
#include <ugdk/util/animationprotocol.h>
#include <ugdk/action/animationframe.h>

#define DEFAULT_FRAME 0
#define DEG_TO_RAD_FACTOR 0.00872664626

namespace ugdk {

// This is the method pointer stored at "entry_functions[RING_TYPE][NAME]".
// Used mostly to make the other macros more readable. Use the one below to call the method.
#define ENTRY_METHOD_PTR(RING_TYPE, NAME) (entry_functions_[ pair<ParsingScope, GDDString>( RING_TYPE , NAME ) ])

// This calls the method stored in "entry_functions_[RING_TYPE][NAME]"
#define ENTRY_METHOD(RING_TYPE, NAME) (this->*ENTRY_METHOD_PTR(RING_TYPE, NAME))

// Pretty much "entry_functions_[RING_TYPE][KEYWORD] = func",
//      where "func" is the function with name ENTRY_FUNCTION_NAME.
// You'll probably want to use the version below this one instead.
#define ENTRY_MAP_ASSIGNMENT(RING_TYPE, KEYWORD, ENTRY_FUNCTION_NAME)                   \
    (ENTRY_METHOD_PTR(RING_TYPE, KEYWORD) = &AnimationProtocol::ENTRY_FUNCTION_NAME)

// Same as above, but assigns 5 keywords. i.e.:
//      entry_functions_[RING_TYPE][KEY_0] = ENTRY_FUNCTION_NAME;
//      entry_functions_[RING_TYPE][KEY_1] = ENTRY_FUNCTION_NAME;
//      etc...
// 5 since there are five standard ids for keywords: "foo", "f".
#define ENTRY_MAP_BULK_ASSIGN(RING_TYPE, ENTRY_FUNCTION, KEY_0, KEY_1) \
    ENTRY_MAP_ASSIGNMENT(RING_TYPE, KEY_0, ENTRY_FUNCTION);            \
    ENTRY_MAP_ASSIGNMENT(RING_TYPE, KEY_1, ENTRY_FUNCTION);

using std::string;
using std::pair;

using gdd::GDDString;
using gdd::GDDArgs;
using gdd::LoadError;

AnimationProtocol::AnimationProtocol() : current_animation_(NULL), current_effect_(NULL), composing_(false) {
    //TODO: Make this map static maybe?

    //ENTRY_MAP_BULK_ASSIGN(EFFECT_RING, NewEntry_EffectNumber  , "number"  , "n");
    ENTRY_MAP_BULK_ASSIGN(EFFECT_RING, NewEntry_EffectAlpha   , "alpha"   , "a");
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
    ENTRY_MAP_BULK_ASSIGN(FRAME_RING , NewEntry_FrameRotation , "rotation", "r");
}

bool AnimationProtocol::NewDescription() {
    if (loader()->data()) {
        loader()->cleanData();
    }
    loader()->newData(new action::AnimationSet);
    return true;
}

bool AnimationProtocol::NewData(const GDDString& data_name) {
    loader()->data()->Add(data_name, current_animation_ = new action::Animation);
    if(current_effect_) delete current_effect_;
    current_effect_ = new graphic::Modifier;
    //TODO: Verificar integridade da current_animation_ .
    return true;
}

bool AnimationProtocol::NewProperty(const GDDString& property_name, const GDDArgs& property_args) {
    if (property_name == "fps" && property_args.size() == 1) {
        int value;
        if (sscanf(property_args[0].c_str(), "%d", &value) != 1) {
            string msg = "Could not read animation fps.";
            error(LoadError::INVALID_VALUE, msg);
            return false;
        }
        current_animation_->set_fps((double)(value));
    } else if (property_name == "compose" && property_args.size() == 0) {
        composing_ = true;
    } else return false;
    return true;
}

bool AnimationProtocol::NewRing(const GDDString& ring_typename) {

    if(ring_typename == "effect") {
        if(!composing_) {
            if(current_effect_) {
                delete current_effect_;
                current_effect_ = NULL;
            }
            current_effect_ = new graphic::Modifier();
        }
        current_scope_ = EFFECT_RING;
    }
    else if(ring_typename == "frame") {
        graphic::Modifier temp_modifier = *current_effect_;
        current_animation_->push_back(new action::AnimationFrame(DEFAULT_FRAME, new graphic::Modifier(temp_modifier)));

        current_scope_ = FRAME_RING;
    }
    else {
        string msg  = "Unkown Ring: \"";
        msg += "TODO: Fix this error message"; //TODO: scope_to_string(current_scope_);
        msg += "\".";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }
    return true;
}

bool AnimationProtocol::NewEntry(const GDDString& entry_name, const GDDArgs& entry_args) {
   /*
    * We don't need to check the scope here, the parser will throw a Syntax Error in case of bad scope,
    * or the protocol will throw a Load Error in the case of Invalid Ring
    */
    std::string lower_name = entry_name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);

    EntryFunction func = ENTRY_METHOD_PTR(current_scope_, lower_name);
    if(func != NULL) {
        return (this->*func)(entry_args);
    } else {
        string msg = "Unknown entry name '" + lower_name + "'.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }
}

bool AnimationProtocol::NewSimpleChain(const GDDString& ring_typename,
                                         const GDDArgs& ring_args) {
    //if (ring_typename == "frames")
        for(size_t i = 0; i < ring_args.size(); i++) {
            GDDArgs entry_arg;
            entry_arg.push_back(ring_args[i]);

            if (!NewRing("frame")) return false;
            if (!NewEntry(ring_typename, entry_arg)) return false;
            /*
            int value;
            if (sscanf(ring_args[i].c_str(), "%d", &value) != 1) {
                string msg = "Could not read animation frame number.";
                error(LoadError::INVALID_VALUE, msg);
                return false;
            }
            Modifier temp_modifier = *current_effect_;
            current_animation_->push_back(
      AnimationFrame(value, new Modifier(temp_modifier))
            );
            */
        }
    //else return false; //TODO: Mensagem da load error.
    return true;
}



// NewEntry Functions
// Effect
bool AnimationProtocol::NewEntry_EffectNumber(const gdd::GDDArgs &args) {

    if( args.size() != 1 || arg_is_not_integer(args[0]) ) {
        string msg = "Invalid argument in an Entry of type Number,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    //TODO: Colocar spreadsheet number no modifier.
    //TODO: implementar esta função..
    return true;

}
bool AnimationProtocol::NewEntry_EffectAlpha(const gdd::GDDArgs &args) {

    if( args.size() != 1 || arg_is_not_doubleing(args[0]) ) {
        string msg = "Invalid argument in an Entry of type Alpha,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    double new_alpha = (double)(atof(args[0].c_str()));
    new_alpha = std::min( std::max(new_alpha,0.0), 1.0 ); // new_alpha is of [0.0,1.0]

    Color c = current_effect_->color();
    c.a = new_alpha;
    current_effect_->set_color(c);
    return true;

}
bool AnimationProtocol::NewEntry_EffectColor(const gdd::GDDArgs &args) {

    if( args.size() != 1 || arg_is_not_hexadecimal(args[0]) ) {
        string msg = "Invalid argument in an Entry of type Color,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    int new_color;
    sscanf(args[0].c_str(), "%x", &new_color);
    int r = (new_color & 0xFF0000) >> 0x10  ,
        g = (new_color & 0x00FF00) >> 0x08  ,
        b =  new_color & 0x0000FF/*>> 0x00*/;

    Color c = Color(r/255.0, g/255.0, b/255.0);

    current_effect_->set_color(c);
    return true;

}
bool AnimationProtocol::NewEntry_EffectPosition(const gdd::GDDArgs &args) {

    if( args.size() != 2 || arg_is_not_doubleing(args[0]) || arg_is_not_doubleing(args[1]) ) {
        string msg = "Invalid argument in an Entry of type Position,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    Vector2D new_pos = Vector2D( (double)(atof(args[0].c_str())), (double)(atof(args[1].c_str())) );

    current_effect_->set_offset(new_pos);
    return true;

}
bool AnimationProtocol::NewEntry_EffectMirror(const gdd::GDDArgs &args) {

    // Mirror with 0 arguments is permitted.
    if( args.size() > 2 || args.size() < 0
        || (args.size() >= 1 && arg_is_not_flip_axis(args[0]) )
        || (args.size() >= 2 && arg_is_not_flip_axis(args[1]) ) ) {
        string msg = "Invalid argument in an Entry of type Mirror,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    Mirror new_mirror = MIRROR_NONE;
    char fst_mirror_id = '0', snd_mirror_id = '0';
    if(args.size() >= 1) {
        fst_mirror_id = static_cast<char>(tolower(args[0].at(0)));
        if(args.size() >= 2) snd_mirror_id = static_cast<char>(tolower(args[1].at(0)));
    }

    if( fst_mirror_id == 'h' || snd_mirror_id == 'h' )
        new_mirror |= MIRROR_HFLIP;
    if( fst_mirror_id == 'v' || snd_mirror_id == 'v' )
        new_mirror |= MIRROR_VFLIP;

    current_effect_->set_mirror(new_mirror);
    return true;

}
bool AnimationProtocol::NewEntry_EffectSize(const gdd::GDDArgs &args) {

    if( args.size() != 2 || arg_is_not_doubleing(args[0]) || arg_is_not_doubleing(args[1]) ) {
        string msg = "Invalid argument in an Entry of type Size,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    Vector2D new_size = Vector2D( (double)(atof(args[0].c_str())), (double)(atof(args[1].c_str())) );

    current_effect_->set_scale(new_size);
    return true;

}
bool AnimationProtocol::NewEntry_EffectRotation(const gdd::GDDArgs &args) {

    if( args.size() != 1 || arg_is_not_doubleing(args[0]) ) {
        string msg = "Invalid argument in an Entry of type Rotation,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    double new_rot  = (double)(atof(args[0].c_str()));
          new_rot *= DEG_TO_RAD_FACTOR;

    current_effect_->set_rotation(new_rot);
    return true;

}
// Frame
bool AnimationProtocol::NewEntry_FrameNumber(const gdd::GDDArgs &args) {

    if( args.size() != 1 || arg_is_not_integer(args[0]) ) {
        string msg = "Invalid argument in an Entry of type Number,\n  in a Ring of type Frame.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    action::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH

    cur_frame->set_frame(atoi(args[0].c_str()));

    return true;

}
bool AnimationProtocol::NewEntry_FrameAlpha(const gdd::GDDArgs &args) {

    if( args.size() != 1 || arg_is_not_doubleing(args[0]) ) {
        string msg = "Invalid argument in an Entry of type Alpha,\n  in a Ring of type Frame.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    double new_alpha = (double)(atof(args[0].c_str()));
    new_alpha = std::min( std::max(new_alpha,0.0), 1.0 ); // new_alpha is of [0.0,1.0]

    action::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH

    Color c = cur_frame->modifier()->color();
    if (composing_) c.a *= new_alpha;
    else            c.a  = new_alpha;
    cur_frame->modifier()->set_color(c);
    return true;

}
bool AnimationProtocol::NewEntry_FrameColor(const gdd::GDDArgs &args) {

    if( args.size() != 1 || arg_is_not_hexadecimal(args[0]) ) {
        string msg = "Invalid argument in an Entry of type Color,\n  in a Ring of type Frame.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    int new_color;
    sscanf(args[0].c_str(), "%x", &new_color);
    int r = (new_color & 0xFF0000) >> 0x10  ,
        g = (new_color & 0x00FF00) >> 0x08  ,
        b =  new_color & 0x0000FF/*>> 0x00*/;

    Color c = Color(r/255.0, g/255.0, b/255.0);

    action::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH
    if (composing_) cur_frame->modifier()->ComposeColor(c);
    else            cur_frame->modifier()->set_color(c);
    return true;

}
bool AnimationProtocol::NewEntry_FramePosition(const gdd::GDDArgs &args) {

    if( args.size() != 2 || arg_is_not_doubleing(args[0]) || arg_is_not_doubleing(args[1]) ) {
        string msg = "Invalid argument in an Entry of type Position,\n  in a Ring of type Frame.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    Vector2D new_pos = Vector2D( (double)(atof(args[0].c_str())), (double)(atof(args[1].c_str())) );

    action::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH
    if (composing_) cur_frame->modifier()->ComposeOffset(new_pos);
    else            cur_frame->modifier()->set_offset(new_pos);
    return true;

}
bool AnimationProtocol::NewEntry_FrameMirror(const gdd::GDDArgs &args) {

    // Mirror with 0 arguments is permitted.
    if( args.size() > 2 || args.size() < 0
        || (args.size() >= 1 && arg_is_not_flip_axis(args[0]) )
        || (args.size() >= 2 && arg_is_not_flip_axis(args[1]) ) ) {
        string msg = "Invalid argument in an Entry of type Mirror,\n  in a Ring of type Frame.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    Mirror new_mirror = MIRROR_NONE;
    char fst_mirror_id = '0', snd_mirror_id = '0';
    if(args.size() >= 1) {
        fst_mirror_id = static_cast<char>(tolower(args[0].at(0)));
        if(args.size() >= 2) snd_mirror_id = static_cast<char>(tolower(args[1].at(0)));
    }

    if( fst_mirror_id == 'h' || snd_mirror_id == 'h' )
        new_mirror |= MIRROR_HFLIP;
    if( fst_mirror_id == 'v' || snd_mirror_id == 'v' )
        new_mirror |= MIRROR_VFLIP;

    action::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH
    if (composing_) cur_frame->modifier()->ComposeMirror(new_mirror);
    else            cur_frame->modifier()->set_mirror(new_mirror);
    return true;

}
bool AnimationProtocol::NewEntry_FrameSize(const gdd::GDDArgs &args) {

    if( args.size() != 2 || arg_is_not_doubleing(args[0]) || arg_is_not_doubleing(args[1]) ) {
        string msg = "Invalid argument in an Entry of type Size,\n  in a Ring of type Frame.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    Vector2D new_size = Vector2D( (double)(atof(args[0].c_str())), (double)(atof(args[1].c_str())) );

    action::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH
    if (composing_) cur_frame->modifier()->ComposeScale(new_size);
    else            cur_frame->modifier()->set_scale(new_size);
    return true;

}
bool AnimationProtocol::NewEntry_FrameRotation(const gdd::GDDArgs &args) {

    if( args.size() != 1 || arg_is_not_doubleing(args[0]) ) {
        string msg = "Invalid argument in an Entry of type Rotation,\n  in a Ring of type Frame.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    double new_rot = (double)(atof(args[0].c_str()));
          new_rot *= DEG_TO_RAD_FACTOR;

    action::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH
    if (composing_) cur_frame->modifier()->ComposeRotation(new_rot);
    else            cur_frame->modifier()->set_rotation(new_rot);
    return true;

}

} /* namespace ugdk */
