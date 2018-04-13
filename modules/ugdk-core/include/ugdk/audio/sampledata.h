#ifndef UGDK_AUDIO_SAMPLE_H_
#define UGDK_AUDIO_SAMPLE_H_

#include <ugdk/structure/types.h>

#include <vector>
#include <utility>

namespace ugdk::audio {

class SampleData {
  public:
    SampleData(U64 size, bool is_stereo);
    void set_sample(U64 index, F64 sample);
    void set_stereo_sample(U64 index, F64 sample_left, F64 sample_right);
    F64 sample(U64 index) const;
    std::pair<F64, F64> stereo_sample(U64 index) const;
    U64 size() const;
    U64 bytes() const;
  private:
    std::vector<F64>    buffer_;
};

SampleData::SampleData(U64 size, bool is_stereo)
    : buffer_(size * (is_stereo ? 2ul : 1ul)) {}

void SampleData::set_sample(U64 index, F64 sample) {
    buffer_[index] = sample;
}

void SampleData::set_stereo_sample(U64 index, F64 sample_left, F64 sample_right) {
    buffer_[2ul * index] = sample_left;
    buffer_[2ul * index + 1] = sample_right;
}

F64 SampleData::sample(U64 index) const {
    return buffer_[index];
}

std::pair<F64, F64> SampleData::stereo_sample(U64 index) const {
    return std::make_pair(buffer_[2ul * index], buffer_[2ul * index + 1]);
}

U64 SampleData::size() const {
    return buffer_.size();
}

U64 SampleData::size() const {
    return buffer_.size() * sizeof(F64);
}

} // ugdk::audio

#endif