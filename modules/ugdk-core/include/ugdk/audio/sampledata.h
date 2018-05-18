
#ifndef UGDK_AUDIO_SAMPLEDATA_H_
#define UGDK_AUDIO_SAMPLEDATA_H_

#include <ugdk/structure/types.h>

#include <vector>
#include <utility>

namespace ugdk {
namespace audio {

class SampleData {
  public:
    SampleData(U64 size, U8 channel_num);
    void set_sample(U64 index, F64 sample);
    void set_channel_sample(U64 index, U8 channel, F64 sample);
    F64 sample(U64 index) const;
    F64 channel_sample(U64 index, U8 channel) const;
    U64 size() const;
    U64 bytes() const;
  private:
    U8                  channel_num_;
    std::vector<F64>    buffer_;
};

inline SampleData::SampleData(U64 size, U8 channel_num)
    : channel_num_(channel_num), buffer_(size * channel_num) {}

inline void SampleData::set_sample(U64 index, F64 sample) {
    buffer_[index] = sample;
}

inline void SampleData::set_channel_sample(U64 index, U8 channel, F64 sample) {
    buffer_[channel_num_ * index + channel] = sample;
}

inline F64 SampleData::sample(U64 index) const {
    return buffer_[index];
}

inline F64 SampleData::channel_sample(U64 index, U8 channel) const {
    return buffer_[channel_num_ * index + channel];
}

inline U64 SampleData::size() const {
    return buffer_.size();
}

inline U64 SampleData::bytes() const {
    return buffer_.size() * sizeof(F64);
}

} // audio
} // ugdk

#endif