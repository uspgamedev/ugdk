
#ifndef UGDK_AUDIO_SAMPLEFRAME_H_
#define UGDK_AUDIO_SAMPLEFRAME_H_

#include <ugdk/structure/types.h>

namespace ugdk::audio {

class SampleFrame {
  public:
    SampleFrame(U64 frame, U64 rate);
        U64 index() const;
        F64 time() const;
  private:
    U64 frame_;
    F64 period_;
};

SampleFrame::SampleFrame(U64 frame, U64 rate)
        : frame_(frame), period_(1.0 / static_cast<F64>(rate)) {}

U64 SampleFrame::index() const {
    return frame_;
}

F64 SampleFrame::time() const {
    return static_cast<F64>(frame_) * period_;
}

} // namespace ugdk::audio

#endif // UGDK_AUDIO_SAMPLEFRAME_H_
