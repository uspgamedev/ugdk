#include <cstdarg>
#include <ugdk/action/animationset.h>

#define ANIMATION_BUFFER_SIZE 256

namespace ugdk {

using namespace std;

AnimationSet::AnimationSet () {}

AnimationSet::~AnimationSet () {}

void AnimationSet::Release() {
    SequenceMap::iterator it = sequences_.begin();
    while (it != sequences_.end()) {
        AnimationManager::Animation *seq = it->second;
        for (size_t i = 0; i < seq->size(); ++i)
            delete seq->at(i);
        ++it;
    }
}

AnimationManager::Animation* AnimationSet::Get(size_t index) {
    return index < indexed_sequences_.size()
        ? indexed_sequences_[index]
        : NULL;
}

void AnimationSet::Add(string name, AnimationManager::Animation *sequence) {
    sequences_[name] = sequence;
}

void AnimationSet::Add(string name, ...) {
    AnimationManager::Animation *sequence = new AnimationManager::Animation();
    va_list arg_list;
    va_start(arg_list, name);
    while (1) {
        int next_arg = va_arg(arg_list, int);
        if (next_arg == -1) { // Indica fim da lista de argumentos
            break;
        }
        sequence->push_back(new AnimationManager::AnimationFrame(next_arg));
    }
    this->Add(name, sequence);
}

AnimationManager::Animation *AnimationSet::Search(std::string name) {
    SequenceMap::iterator it = sequences_.find(name);
    return it == sequences_.end()?
            NULL :
            it->second;
}

uint32 AnimationSet::MakeIndex(string name) {
    AnimationManager::Animation *sequence = this->Search(name);
    if (sequence == NULL) return -1;
    for (size_t i = 0; i < indexed_sequences_.size(); ++i)
        if (indexed_sequences_[i] == sequence)
            return i;
    indexed_sequences_.push_back(sequence);
    return static_cast<uint32>(indexed_sequences_.size()-1);
}

void AnimationSet::Print(FILE *out) {
	fprintf(out, "Printing animation set.\n");
	SequenceMap::iterator it = sequences_.begin();
	while (it != sequences_.end()) {
		AnimationManager::Animation *seq = it->second;
		fprintf(out, "%s:", it->first.c_str());
		for (size_t i = 0; i < seq->size(); ++i)
			fprintf(out, " %d", seq->at(i)->frame());
		fputc(10, out);
		++it;
	}
}

}
