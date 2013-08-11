#ifndef UGDK_STRUCTURE_H_
#define UGDK_STRUCTURE_H_

namespace ugdk {
namespace structure {
  
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

template<class T>
class IndexableTable;

}  // namespace structures
}  // namespace ugdk

#endif // UGDK_STRUCTURE_H_
