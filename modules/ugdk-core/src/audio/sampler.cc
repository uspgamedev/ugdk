Sampler::Sampler() {}
Sampler::Sampler(ALsizei size, AudioFormat form, const std::function<void(unsigned long long)>& gen_func)
    : size_(size)
    , form_(form)
    , gen_func_(gen_func)
    , offset_(0)
    , buffer_(size)
{
    alGenBuffers(1, &name_);
}
Sampler::~Sampler();
virtual void Sampler::GetSample();
void Sampler::Rewind();
