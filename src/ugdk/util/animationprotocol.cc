
#include <string>
#include <algorithm>
#include <ugdk/util/animationprotocol.h>

#define DEFAULT_FRAME 0
#define DEG_TO_RAD_FACTOR 0.00872664626f

namespace ugdk {

#define ENTRY_METHOD(RING_TYPE, NAME) (this->*entry_functions_[ pair<ParsingScope, GDDString>( RING_TYPE , NAME ) ])

using std::string;
using std::pair;

using gdd::GDDString;
using gdd::GDDArgs;
using gdd::LoadError;

AnimationProtocol::AnimationProtocol() : current_animation_(NULL), current_effect_(NULL), composing_(false) {
    //TODO: Make this map static maybe?
    //entry_functions_[ pair<ParsingScope, GDDString>(EFFECT_RING, "number")   ] = &AnimationProtocol::NewEntry_EffectNumber;
    entry_functions_[ pair<ParsingScope, GDDString>(EFFECT_RING, "alpha")    ] = &AnimationProtocol::NewEntry_EffectAlpha;
    entry_functions_[ pair<ParsingScope, GDDString>(EFFECT_RING, "color")    ] = &AnimationProtocol::NewEntry_EffectColor;
    entry_functions_[ pair<ParsingScope, GDDString>(EFFECT_RING, "position") ] = &AnimationProtocol::NewEntry_EffectPosition;
    entry_functions_[ pair<ParsingScope, GDDString>(EFFECT_RING, "mirror")   ] = &AnimationProtocol::NewEntry_EffectMirror;
    entry_functions_[ pair<ParsingScope, GDDString>(EFFECT_RING, "scale")     ] = &AnimationProtocol::NewEntry_EffectSize;
    entry_functions_[ pair<ParsingScope, GDDString>(EFFECT_RING, "rotation") ] = &AnimationProtocol::NewEntry_EffectRotation;

    entry_functions_[ pair<ParsingScope, GDDString>(FRAME_RING, "number")   ] = &AnimationProtocol::NewEntry_FrameNumber;
    entry_functions_[ pair<ParsingScope, GDDString>(FRAME_RING, "alpha")    ] = &AnimationProtocol::NewEntry_FrameAlpha;
    entry_functions_[ pair<ParsingScope, GDDString>(FRAME_RING, "color")    ] = &AnimationProtocol::NewEntry_FrameColor;
    entry_functions_[ pair<ParsingScope, GDDString>(FRAME_RING, "position") ] = &AnimationProtocol::NewEntry_FramePosition;
    entry_functions_[ pair<ParsingScope, GDDString>(FRAME_RING, "mirror")   ] = &AnimationProtocol::NewEntry_FrameMirror;
    entry_functions_[ pair<ParsingScope, GDDString>(FRAME_RING, "size")     ] = &AnimationProtocol::NewEntry_FrameSize;
    entry_functions_[ pair<ParsingScope, GDDString>(FRAME_RING, "rotation") ] = &AnimationProtocol::NewEntry_FrameRotation;
}

bool AnimationProtocol::NewDescription() {
    if (loader()->data()) {
        loader()->cleanData();
    }
    loader()->newData(new AnimationSet);
    return true;
}

bool AnimationProtocol::NewData(const GDDString& data_name) {
    loader()->data()->Add(data_name, current_animation_ = new AnimationManager::Animation);
    if(current_effect_) delete current_effect_;
    current_effect_ = new Modifier;
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
        current_animation_->set_fps(value);
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
            current_effect_ = new Modifier();
        }
        current_scope_ = EFFECT_RING;
    }
    else if(ring_typename == "frame") {
        Modifier temp_modifier = *current_effect_;
        current_animation_->push_back(new AnimationManager::AnimationFrame(DEFAULT_FRAME,new Modifier(temp_modifier)));

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
    ENTRY_METHOD(current_scope_, entry_name)(entry_args);

    if (0) {
        //alpha
        switch (current_scope_) {
          case EFFECT_RING: {
            Modifier* temp_modifier = new Modifier();
            temp_modifier->set_alpha(atof(entry_args[0].c_str()));

            
            if(composing_) {
                current_effect_->Compose(temp_modifier);
                

            } else {
                current_effect_->set_alpha(temp_modifier->alpha());
            }
            delete temp_modifier;
            break;
          }
          case FRAME_RING: {
            AnimationManager::AnimationFrame* cur_frame
                = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH

            Modifier* temp_modifier = new Modifier();
            temp_modifier->set_alpha(atof(entry_args[0].c_str()));

            cur_frame->modifier()->Compose(temp_modifier);

            delete temp_modifier;
            break;
          }
        }
    }

    return true;
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
                    new AnimationManager::AnimationFrame(value, new Modifier(temp_modifier))
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

    if( args.size() != 1 || arg_is_not_floating(args[0]) ) {
        string msg = "Invalid argument in an Entry of type Alpha,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    float new_alpha = atof(args[0].c_str());
    new_alpha = std::min( std::max(new_alpha,0.0f), 1.0f ); // new_alpha is of [0.0f,1.0f]

    current_effect_->set_alpha(new_alpha);
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

    Color c = Color(r/255.0f, g/255.0f, b/255.0f);

    current_effect_->set_color(c);
    return true;

}
bool AnimationProtocol::NewEntry_EffectPosition(const gdd::GDDArgs &args) {

    if( args.size() != 2 || arg_is_not_floating(args[0]) || arg_is_not_floating(args[1]) ) {
        string msg = "Invalid argument in an Entry of type Position,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    Vector2D new_pos = Vector2D( atof(args[0].c_str()), atof(args[1].c_str()) );

    current_effect_->set_offset(new_pos);
    return true;

}
bool AnimationProtocol::NewEntry_EffectMirror(const gdd::GDDArgs &args) {

    // Mirror with 0 arguments is permitted:
    if( args.size() == 0 ) return true;

    if( args.size() > 2 || args.size() < 0
        || arg_is_not_flip_axis(args[0])
        || (args.size() == 2 && arg_is_not_flip_axis(args[1]) ) ) {
        string msg = "Invalid argument in an Entry of type Mirror,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    Mirror new_mirror = MIRROR_NONE;
    char fst_mirror_id = '0', snd_mirror_id = '0';
    if(args.size() >= 1) {
        fst_mirror_id = tolower(args[0].at(0));
        if(args.size() >= 2) snd_mirror_id = tolower(args[1].at(0));
    }

    if( fst_mirror_id == 'h' || snd_mirror_id == 'h' )
        new_mirror |= MIRROR_HFLIP;
    if( fst_mirror_id == 'v' || snd_mirror_id == 'v' )
        new_mirror |= MIRROR_VFLIP;

    current_effect_->set_mirror(new_mirror);
    return true;

}
bool AnimationProtocol::NewEntry_EffectSize(const gdd::GDDArgs &args) {

    if( args.size() != 2 || arg_is_not_floating(args[0]) || arg_is_not_floating(args[1]) ) {
        string msg = "Invalid argument in an Entry of type Size,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    Vector2D new_size = Vector2D( atof(args[0].c_str()), atof(args[1].c_str()) );

    current_effect_->set_scale(new_size);
    return true;

}
bool AnimationProtocol::NewEntry_EffectRotation(const gdd::GDDArgs &args) {

    if( args.size() != 1 || arg_is_not_floating(args[0]) ) {
        string msg = "Invalid argument in an Entry of type Rotation,\n  in a Ring of type Effect.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    float new_rot  = atof(args[0].c_str());
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

    AnimationManager::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH

    cur_frame->set_frame(atoi(args[0].c_str()));

    return true;

}
bool AnimationProtocol::NewEntry_FrameAlpha(const gdd::GDDArgs &args) {

    if( args.size() != 1 || arg_is_not_floating(args[0]) ) {
        string msg = "Invalid argument in an Entry of type Alpha,\n  in a Ring of type Frame.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    float new_alpha = atof(args[0].c_str());
    new_alpha = std::min( std::max(new_alpha,0.0f), 1.0f ); // new_alpha is of [0.0f,1.0f]

    AnimationManager::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH

    if (composing_) cur_frame->modifier()->ComposeAlpha(new_alpha);
    else            cur_frame->modifier()->set_alpha(new_alpha);
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

    Color c = Color(r/255.0f, g/255.0f, b/255.0f);

    AnimationManager::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH
    if (composing_) cur_frame->modifier()->ComposeColor(c);
    else            cur_frame->modifier()->set_color(c);
    return true;

}
bool AnimationProtocol::NewEntry_FramePosition(const gdd::GDDArgs &args) {

    if( args.size() != 2 || arg_is_not_floating(args[0]) || arg_is_not_floating(args[1]) ) {
        string msg = "Invalid argument in an Entry of type Position,\n  in a Ring of type Frame.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    Vector2D new_pos = Vector2D( atof(args[0].c_str()), atof(args[1].c_str()) );

    AnimationManager::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH
    if (composing_) cur_frame->modifier()->ComposeOffset(new_pos);
    else            cur_frame->modifier()->set_offset(new_pos);
    return true;

}
bool AnimationProtocol::NewEntry_FrameMirror(const gdd::GDDArgs &args) {

    // Mirror with 0 arguments is permitted:
    if( args.size() == 0 ) return true;

    if( args.size() > 2 || args.size() < 0
        || arg_is_not_flip_axis(args[0])
        || (args.size() == 2 && arg_is_not_flip_axis(args[1]) ) ) {
        string msg = "Invalid argument in an Entry of type Mirror,\n  in a Ring of type Frame.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    Mirror new_mirror = MIRROR_NONE;
    char fst_mirror_id = '0', snd_mirror_id = '0';
    if(args.size() >= 1) {
        fst_mirror_id = tolower(args[0].at(0));
        if(args.size() >= 2) snd_mirror_id = tolower(args[1].at(0));
    }

    if( fst_mirror_id == 'h' || snd_mirror_id == 'h' )
        new_mirror ^= MIRROR_HFLIP;
    if( fst_mirror_id == 'v' || snd_mirror_id == 'v' )
        new_mirror ^= MIRROR_VFLIP;

    AnimationManager::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH
    if (composing_) cur_frame->modifier()->ComposeMirror(new_mirror);
    else            cur_frame->modifier()->set_mirror(new_mirror);
    return true;

}
bool AnimationProtocol::NewEntry_FrameSize(const gdd::GDDArgs &args) {

    if( args.size() != 2 || arg_is_not_floating(args[0]) || arg_is_not_floating(args[1]) ) {
        string msg = "Invalid argument in an Entry of type Size,\n  in a Ring of type Frame.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    Vector2D new_size = Vector2D( atof(args[0].c_str()), atof(args[1].c_str()) );

    AnimationManager::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH
    if (composing_) cur_frame->modifier()->ComposeScale(new_size);
    else            cur_frame->modifier()->set_scale(new_size);
    return true;

}
bool AnimationProtocol::NewEntry_FrameRotation(const gdd::GDDArgs &args) {

    if( args.size() != 1 || arg_is_not_floating(args[0]) ) {
        string msg = "Invalid argument in an Entry of type Rotation,\n  in a Ring of type Frame.";
        error(LoadError::INVALID_VALUE, msg);
        return false;
    }

    float new_rot = atof(args[0].c_str());
          new_rot *= DEG_TO_RAD_FACTOR;

    AnimationManager::AnimationFrame* cur_frame
        = current_animation_->at(current_animation_->size() - 1); // Current Frame. YEEEAAAHHHHHHH
    if (composing_) cur_frame->modifier()->ComposeRotation(new_rot);
    else            cur_frame->modifier()->set_rotation(new_rot);
    return true;

}

} /* namespace ugdk */
