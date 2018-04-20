
#ifndef UGDK_AUDIO_SAMPLEFRAME_H_
#define UGDK_AUDIO_SAMPLEFRAME_H_

#include <ugdk/structure/types.h>

#include <vector>

namespace ugdk::audio {

class SampleFrame {
  public:
    SampleFrame(U64 frame, U64 rate, U8 channel_num);
    U64 index() const;
    F64 time() const;
    void set_channel(size_t channel, F64 value);
    void set(F64 value);
  private:
    U64 frame_;
    F64 period_;
    std::vector<F64> values_;
};

SampleFrame::SampleFrame(U64 frame, U64 rate, U8 channel_num)
    : frame_(frame), period_(1.0 / static_cast<F64>(rate)), values_(channel_num, 0.0) {}

U64 SampleFrame::index() const {
    return frame_;
}

F64 SampleFrame::time() const {
    return static_cast<F64>(frame_) * period_;
}

void SampleFrame::set_channel(size_t channel, F64 value) {
    assert(channel < values_.size());
    values_[channel] = value;
}

void SampleFrame::set(F64 value) {
    values_[0] = value;
}

} // namespace ugdk::audio

#endif // UGDK_AUDIO_SAMPLEFRAME_H_
