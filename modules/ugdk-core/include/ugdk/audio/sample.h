#ifndef UGDK_AUDIO_SAMPLER_H_
#define UGDK_AUDIO_SAMPLER_H_

#include <vector>
#include <ugdk/structure/types.h>

namespace ugdk {
namespace audio {

struct AbstractSampleData {
    virtual bool is_stereo() const = 0;
}

template<typename SampleType, bool is_stereo>
class SampleData;

template<bool is_stereo>
class SampleData<I8, is_stereo>  : public AbstractSampleData {
    std::vector<I8> data_;  
    bool is_stereo() const {return is_stereo;}    
};

template<bool is_stereo>
class SampleData<I16, is_stereo> : public AbstractSampleData {
    std::vector<I16> data_;
    bool is_stereo() const {return is_stereo;}
};

} // audio
} // ugdk

#endif