

#ifndef HORUSEYE_FRAMEWORK_ANIMATIONPARSER_H_
#define HORUSEYE_FRAMEWORK_ANIMATIONPARSER_H_

#include <map>
#include <string>

#define ANIMATION_PARSER() (ugdk::AnimationParser::reference())

namespace ugdk {

namespace action{
class AnimationSet;
}

class AnimationParser {
  public:

    ~AnimationParser ();

    static AnimationParser* reference();

    action::AnimationSet* Load(const std::string& file_path);

  private:

    AnimationParser () {}

    action::AnimationSet* parse(const std::string& file_path);

    static AnimationParser *reference_;

    std::map<std::string, action::AnimationSet*> animation_sets_;

};

}

#endif /* HORUSEYE_FRAMEWORK_ANIMATIONPARSER_H_ */
