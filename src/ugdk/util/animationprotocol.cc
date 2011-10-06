
#include <string>

#include <ugdk/util/animationprotocol.h>

namespace ugdk {

using std::string;

using gdd::GDDString;
using gdd::GDDArgs;
using gdd::LoadError;

bool AnimationProtocol::NewDescription() {
    if (loader()->data()) {
        loader()->cleanData();
    }
    loader()->newData(new AnimationSet);
    return true;
}

bool AnimationProtocol::NewData(const GDDString& data_name) {
    loader()->data()->Add(data_name, current_animation_ = new AnimationManager::Animation);
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
    } else return false;
    return true;
}

bool AnimationProtocol::NewRing(const GDDString& ring_typename) {
    return true;
}

bool AnimationProtocol::NewEntry(const GDDString& entry_name, const GDDArgs& entry_args) {
    return true;
}

bool AnimationProtocol::NewSimpleChain(const GDDString& ring_typename,
                                         const GDDArgs& ring_args) {
    if (current_animation_ && ring_typename == "frames")
        for(size_t i = 0; i < ring_args.size(); i++) {
            int value;
            if (sscanf(ring_args[i].c_str(), "%d", &value) != 1) {
                string msg = "Could not read animation frame number.";
                error(LoadError::INVALID_VALUE, msg);
                return false;
            }
            current_animation_->push_back(new AnimationManager::AnimationFrame(value));
        }
    else return false;
    return true;
}

} /* namespace ugdk */
