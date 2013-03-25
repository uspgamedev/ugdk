#ifndef UGDK_UTIL_H_
#define UGDK_UTIL_H_

namespace ugdk {
//namespace util {

class AnimationParser;
class AnimationProtocol;
class Language;
class LanguageManager;
class LanguageWord;
class PathManager;
   
namespace ikdtree {
    template <class T, int DIMENSIONS>
    class IntervalKDTree;
    
    template <int DIMENSIONS>
    class Box;
    
    template <class T, int DIMENSIONS>
    class Node;
    
    template <class T, int DIMENSIONS>
    class Item;
}


namespace util {

template<class T>
class IndexableTable;

}

//}  // namespace util
}  // namespace ugdk

#endif
