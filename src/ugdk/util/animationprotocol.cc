
#include <string>
#include <ugdk/util/animationprotocol.h>

#define DEFAULT_FRAME 0

namespace ugdk {

using std::string;
using std::pair;

using gdd::GDDString;
using gdd::GDDArgs;
using gdd::LoadError;

AnimationProtocol::AnimationProtocol() : current_animation_(NULL), current_effect_(NULL), composing_(false) {
    entry_functions_[ pair<ParsingScope, GDDString>(EFFECT_RING, "alpha") ] = &AnimationProtocol::NewEntry_EffectAlpha;
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
            if(current_effect_) delete current_effect_;
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
    if (1) {
        //alpha
        switch (current_scope_) {
          case EFFECT_RING: {
            Modifier* temp_modifier = new Modifier();
            temp_modifier->set_alpha(atof(entry_args[0].c_str()));


            if(composing_) {
                current_effect_->Compose(temp_modifier);
                delete temp_modifier;

            } else {
                if(current_effect_) delete current_effect_;
                current_effect_ = temp_modifier;
            }
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
    if (ring_typename == "frames")
        for(size_t i = 0; i < ring_args.size(); i++) {
            int value;
            if (sscanf(ring_args[i].c_str(), "%d", &value) != 1) {
                string msg = "Could not read animation frame number.";
                error(LoadError::INVALID_VALUE, msg);
                return false;
            }
            current_animation_->push_back(new AnimationManager::AnimationFrame(value));
        }
    else return false; //TODO: Mensagem da load error.
    return true;
}

} /* namespace ugdk */
