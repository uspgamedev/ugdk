#include <cstdarg>
#include <cstdio>
#include <ugdk/action/animationset.h>
#include <ugdk/action/animation.h>

#define ANIMATION_BUFFER_SIZE 256

namespace ugdk {

namespace action {

AnimationSet::AnimationSet () {}

AnimationSet::~AnimationSet () {}

void AnimationSet::Release() {
    SequenceMap::iterator it = sequences_.begin();
    while (it != sequences_.end()) {
        Animation *seq = it->second;
        for (size_t i = 0; i < seq->size(); ++i)
            delete seq->at(i);
        ++it;
    }
}

Animation* AnimationSet::Get(int index) {
    return static_cast<size_t>(index) < indexed_sequences_.size()
        ? indexed_sequences_[index]
        : NULL;
}

void AnimationSet::Add(const std::string& name, Animation *sequence) {
    sequences_[name] = sequence;
}

void AnimationSet::Add(const std::string& name, ...) {
    Animation *sequence = new Animation();
    va_list arg_list;
    va_start(arg_list, name);
    while (1) {
        int next_arg = va_arg(arg_list, int);
        if (next_arg == -1) { // Indica fim da lista de argumentos
            break;
        }
        sequence->push_back(new AnimationFrame(next_arg));
    }
    this->Add(name, sequence);
}

Animation *AnimationSet::Search(const std::string& name) {
    SequenceMap::iterator it = sequences_.find(name);
    return it == sequences_.end()?
            NULL :
            it->second;
}

int AnimationSet::MakeIndex(const std::string& name) {
    Animation *sequence = this->Search(name);
    if (sequence == NULL) return -1;
    for (size_t i = 0; i < indexed_sequences_.size(); ++i)
        if (indexed_sequences_[i] == sequence)
            return static_cast<int>(i);
    indexed_sequences_.push_back(sequence);
    return static_cast<int>(indexed_sequences_.size()-1);
}

void AnimationSet::Print(FILE *out) {
	fprintf(out, "Printing animation set.\n");
	SequenceMap::iterator it = sequences_.begin();
	while (it != sequences_.end()) {
		Animation *seq = it->second;
		fprintf(out, "%s:", it->first.c_str());
		for (size_t i = 0; i < seq->size(); ++i)
			fprintf(out, " %d", seq->at(i)->frame());
		fputc(10, out);
		++it;
	}
}

} /* namespace action */

} /* namespace ugdk */
