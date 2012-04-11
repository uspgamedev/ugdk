#include <cstdio>
#include <cstring>
#include <ugdk/util/animationparser.h>
#include <ugdk/action/animation.h>
#include <ugdk/action/animationset.h>

namespace ugdk {

using namespace std;

AnimationParser* AnimationParser::reference_ = NULL;

AnimationParser::~AnimationParser () {
    std::map<std::string, action::AnimationSet*>::iterator it;
    it = animation_sets_.begin();
    while (it != animation_sets_.end()) {
		it->second->Release();
		delete it->second;
		++it;
    }
    reference_ = NULL;
}

AnimationParser* AnimationParser::reference() {
    return reference_ ? reference_ : (reference_ = new AnimationParser);
}

bool parseLine(char buffer[], action::AnimationSet *set) {
    static action::Animation *seq = NULL;
    static char animation_name[256];
    char *token = buffer;
    size_t len = strlen(buffer);
    if (buffer[len-1] == '\n') buffer[len-1] = '\0';
    while (*token != '\0' && *token != '#') {
        switch (*token) {
        case '$':
        	sscanf(token+1, "%s", animation_name);
        	//printf("New animation: %s\n", animation_name);
	    set->Add(animation_name, seq = new action::Animation);
            return true;
        case '%':
            if (seq) {
                int frame = 0, step = 0;
                token++;
                while (sscanf(token, "%d%*[ \t]%n", &frame, &step) > 0) {
		    seq->push_back(new action::AnimationFrame(frame));
                    token += step;
                }
                if (*token != '\0') {
                	puts("Unknown input. 2");
                	puts(token);
                    return false; /* TODO error msg */
                }
                else return true;
            }
            else {
            	puts("No animation defined.");
            	return false; /* TODO error msg */
            }
            break;
        case ' ':
        case '\t':
        case '\n':
            token++;
            break;
        default:
        	puts("Unknown input.");
            return false; /* TODO error msg */
        }
    }
    return true;
}

action::AnimationSet* parseFile(FILE *file) {
    static char buffer[1024];
    action::AnimationSet *set = new action::AnimationSet;
    buffer[0] = '\0';
    while (fgets(buffer, 1024, file))
        if(!parseLine(buffer, set)) {
            set->Release();
            delete set;
            return NULL;
        }
    return set;
}

action::AnimationSet* AnimationParser::parse(const string& file_path) {
    FILE *file = fopen(file_path.c_str(), "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", file_path.c_str());
        return NULL;
    }
    return parseFile(file);
}

action::AnimationSet* AnimationParser::Load(const std::string& file_path) {
  map<string, action::AnimationSet*>::iterator it = animation_sets_.find(file_path);
    if (it == animation_sets_.end()) {
        action::AnimationSet *set = parse(file_path);
        if (!set) return NULL;
        animation_sets_[file_path] = set;
        return set;
    }
    else return it->second;
}

}
