
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
    loader()->data()->Add(data_name, current_seq_ = new Animation::FrameSequence);
    return true;
}

bool AnimationProtocol::NewProperty(const GDDString& property_name, const GDDArgs& property_args) {
    return true;
}

bool AnimationProtocol::NewSegment(const GDDString& segment_typename) {
    return true;
}

bool AnimationProtocol::NewEntry(const GDDString& entry_name, const GDDArgs& entry_args) {
    return true;
}

bool AnimationProtocol::NewSimpleSegment(const GDDString& segment_typename,
                                         const GDDArgs& segment_args) {
    if (segment_typename == "frames")
        for(size_t i = 0; i < segment_args.size(); i++) {
            int value;
            if (sscanf(segment_args[i].c_str(), "%d", &value) != 1) {
                string msg = "Could not read animation frame number.";
                error(LoadError::INVALID_VALUE, msg);
                return false;
            }
            current_seq_->push_back(new Animation::AnimationFrame(value));
        }
    else return false;
    return true;
}

} /* namespace ugdk */
