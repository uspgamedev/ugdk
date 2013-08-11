#ifndef UGDK_UTIL_INTERVALKDTREE_H_
#define UGDK_UTIL_INTERVALKDTREE_H_

#include <cassert>
#include <cstddef>
#include <map>
#include <list>
#include <vector>
#include <climits>
#ifdef DEBUG
#include <iostream>
#endif

#include <ugdk/structure.h>
#include <ugdk/structure/box.h>

namespace ugdk {
namespace structure {
namespace ikdtree {

template <class T, int DIMENSIONS>
class IntervalKDTree {
  public:
    IntervalKDTree(const Box<DIMENSIONS>& tree_bounding_box,
                   unsigned int max_elements_per_leaf);
    ~IntervalKDTree ();
    
    void Clear();
    void Insert(const Box<DIMENSIONS>& bounding_box, const T& element);
    void Remove(const T& element);
    void Update(const Box<DIMENSIONS>& new_bounding_box, const T& element);

    std::vector<T>* getIntersectingItems (const Box<DIMENSIONS>& boundary) const;
    
    unsigned int max_elements_per_leaf() const;

#ifdef DEBUG
    unsigned int max_height_;
    void PrintTree() const;
#endif

  private:
    void UpdateItem (Item<T, DIMENSIONS> * item);

    unsigned int max_elements_per_leaf_;
    Box<DIMENSIONS> tree_bounding_box_;
    std::map<T, Item<T, DIMENSIONS>* > container_items_;
    Node<T, DIMENSIONS> * root_;
};

//************* Implementation **************

//**** Classes needed for implementation ****

template <class T, int DIMENSIONS>
class Item : public Box<DIMENSIONS> {
  public :
    Item(const Box<DIMENSIONS>& bounding_box, const T& element);
    ~Item();
    
    const T& element() const;
    
    void set_container_node(Node<T, DIMENSIONS>* container_node);
    Node<T, DIMENSIONS>* container_node() const;

#ifdef DEBUG
    void Print() const;
#endif

  private:
    typedef Box<DIMENSIONS> super;
    T element_;
    Node<T, DIMENSIONS>* container_node_;
};

template <class T, int DIMENSIONS>
class Node : public Box<DIMENSIONS> {
  public:
    Node(IntervalKDTree<T, DIMENSIONS> * tree, Node *parent, int depth,
        const std::array<Coordinate, DIMENSIONS>& min_coordinates, 
        const std::array<Coordinate, DIMENSIONS>& max_coordinates);
    Node(IntervalKDTree<T, DIMENSIONS> * tree, Node *parent, int depth,
                const Box<DIMENSIONS>& coordinates);
    ~Node ();
    
    void InsertItem(Item<T, DIMENSIONS> *item);
    void RemoveItem(Item<T, DIMENSIONS> *item);
    void getIntersectingItems(const Box<DIMENSIONS>& boundary, std::vector<T> *items) const;
    
    void Clear();

    int depth_;
    Coordinate division_boundary_;
    bool has_children_;

#ifdef DEBUG
    void PrintTreeRootedAtThisNode();
#endif

  private:
    void Divide();
    void Merge();

    typedef Box<DIMENSIONS> super;
    IntervalKDTree<T, DIMENSIONS> *tree_;
    Node * parent_;
    Node * low_child_, * high_child_;

    // TODO: Test if some kind of tree is faster than a list
    // Maybe an interval tree along one of the other axis (not
    // on the axis that this node divides)
    // Maybe even another KD-Interval-Tree. (but taking care to not
    // have infinite recursive trees)
    std::list<Item<T, DIMENSIONS> *> items_;
};

// IntervalKDTree implementation

template <class T, int DIMENSIONS>
IntervalKDTree<T, DIMENSIONS>::IntervalKDTree (
                                    const Box<DIMENSIONS>& _tree_bounding_box,
                                    unsigned int _max_elements_per_leaf) :
    max_elements_per_leaf_(_max_elements_per_leaf), 
    tree_bounding_box_(_tree_bounding_box), 
    container_items_() {
        root_ = new Node<T, DIMENSIONS>(this, nullptr, 0, _tree_bounding_box);
#ifdef DEBUG
    max_height_ = 0;
#endif
}

template <class T, int DIMENSIONS>
IntervalKDTree<T, DIMENSIONS>::~IntervalKDTree () {
    root_->Clear ();
    delete root_;
}

template <class T, int DIMENSIONS>
void IntervalKDTree<T, DIMENSIONS>::Clear () {
    container_items_.clear ();
    root_->Clear ();
    delete root_;
    root_ = new Node<T,DIMENSIONS> (this, nullptr, 0, tree_bounding_box_);
}

template <class T, int DIMENSIONS>
void IntervalKDTree<T, DIMENSIONS>::Insert (const Box<DIMENSIONS>& bounding_box, const T& element) {
    // Shouldn't insert an element twice!
    assert (!container_items_.count (element));
    Item<T, DIMENSIONS> * item = new Item<T, DIMENSIONS> (bounding_box, element);
    root_->InsertItem (item);
    container_items_[element] = item;
}

template <class T, int DIMENSIONS>
void IntervalKDTree<T, DIMENSIONS>::Remove (const T& element) {
    assert (container_items_.count (element));
    if (container_items_.count (element)) {
        Item<T, DIMENSIONS> *item = container_items_[element];
        Node<T, DIMENSIONS> *node = item->container_node();
        node->RemoveItem (item);
        container_items_.erase (element);
        delete item;
    }
}

template <class T, int DIMENSIONS>
void IntervalKDTree<T, DIMENSIONS>::Update (const Box<DIMENSIONS>& new_bounding_box,
        const T& element) {
    // Shouldn't update an unexisting element!
    assert (container_items_.count(element));
    Item<T, DIMENSIONS> * item = container_items_[element];
    item->UpdateBox (new_bounding_box);
    UpdateItem (item);
}

template <class T, int DIMENSIONS>
void IntervalKDTree<T, DIMENSIONS>::UpdateItem(Item<T, DIMENSIONS> * item) {
    Node<T, DIMENSIONS> *container_node = item->container_node ();

    if ((container_node->has_children_ &&
             (item->IsBelow (container_node->depth_,
                             container_node->division_boundary_)
              || item->IsAbove (container_node->depth_,
                                container_node->division_boundary_))
        ) || !container_node->Contains (*item)) {
        // We need to place it in a different node
        // Remove and add item
        container_node->RemoveItem (item);
        root_->InsertItem (item);
    }
}

template <class T, int DIMENSIONS>
std::vector<T>* IntervalKDTree<T, DIMENSIONS>::getIntersectingItems (
        const Box<DIMENSIONS>& boundary) const {
    std::vector<T>* items = new std::vector<T>;
    root_->getIntersectingItems (boundary, items);
    return items;
}

template <class T, int DIMENSIONS>
unsigned int IntervalKDTree<T, DIMENSIONS>::max_elements_per_leaf() const {
    return max_elements_per_leaf_;
}

#ifdef DEBUG
template <class T, int DIMENSIONS>
void IntervalKDTree<T, DIMENSIONS>::PrintTree() const {
    root_->PrintTreeRootedAtThisNode ();
}
#endif

// Item Implementation

template <class T, int DIMENSIONS>
Item<T, DIMENSIONS>::Item(const Box<DIMENSIONS>& bounding_box, const T& _element) :
    super(bounding_box), element_(_element) {}

template <class T, int DIMENSIONS>
Item<T, DIMENSIONS>::~Item() {}

template <class T, int DIMENSIONS>
Node<T, DIMENSIONS>* Item<T, DIMENSIONS>::container_node() const {
    return container_node_;
}

template <class T, int DIMENSIONS>
void Item<T, DIMENSIONS>::set_container_node(Node<T, DIMENSIONS>* _container_node) {
    container_node_ = _container_node;
}

template <class T, int DIMENSIONS>
const T& Item<T, DIMENSIONS>::element() const {
    return element_;
}

#ifdef DEBUG
template <class T, int DIMENSIONS>
void Item<T, DIMENSIONS>::Print() const {
    std::cout << element_;
}
#endif

// Node Implementation

template <class T, int DIMENSIONS>
Node<T, DIMENSIONS>::Node(IntervalKDTree<T, DIMENSIONS> * tree, Node *parent, int depth, 
        const std::array<Coordinate, DIMENSIONS>& min_coordinates, 
        const std::array<Coordinate, DIMENSIONS>& max_coordinates)

  : super (min_coordinates, max_coordinates), depth_(depth), has_children_(false), 
    tree_(tree), parent_(parent), low_child_(nullptr), high_child_(nullptr) {}

template <class T, int DIMENSIONS>
Node<T, DIMENSIONS>::Node (IntervalKDTree<T, DIMENSIONS> * tree,
    Node *parent, int depth, const Box<DIMENSIONS>& coordinates) 
    
  : super (coordinates), depth_(depth), has_children_(false), 
    tree_(tree), parent_(parent), low_child_(nullptr), high_child_(nullptr) {}

template <class T, int DIMENSIONS>
Node<T, DIMENSIONS>::~Node () {
    delete low_child_;
    delete high_child_;
}

template <class T, int DIMENSIONS>
void Node<T, DIMENSIONS>::InsertItem (Item<T, DIMENSIONS> *item) {
#ifdef DEBUG
    if (static_cast<unsigned int>(depth_) > tree_->max_height_) {
        tree_->max_height_ = depth_;
    }
#endif
    if (has_children_) {
        if (item->IsBelow (depth_, division_boundary_)) {
            assert (low_child_);
            low_child_->InsertItem (item);
            return;
        } else if (item->IsAbove (depth_, division_boundary_)) {
            assert (high_child_);
            high_child_->InsertItem (item);
            return;
        }
    }
    items_.push_back (item);
    item->set_container_node (this);
    if (!has_children_ && items_.size() > tree_->max_elements_per_leaf()) {
        Divide ();
    }
}

template <class T, int DIMENSIONS>
void Node<T, DIMENSIONS>::RemoveItem (Item<T, DIMENSIONS> *item) {
    items_.remove (item);
    if (parent_)
        parent_->Merge ();
}

template <class T, int DIMENSIONS>
void Node<T, DIMENSIONS>::getIntersectingItems (const Box<DIMENSIONS>& boundary,
        std::vector<T> *intersecting_items) const {
    for (typename std::list<Item<T, DIMENSIONS> *>::const_iterator it = items_.begin();
            it != items_.end(); ++it) {
        if (boundary.Intersects (*it)) {
            intersecting_items->push_back ((*it)->element ());
        }
    }
    if (has_children_) {
        if (boundary.IsBelow (depth_, division_boundary_)) {
            low_child_->getIntersectingItems (boundary, intersecting_items);
        } else if (boundary.IsAbove (depth_, division_boundary_)) {
            high_child_->getIntersectingItems (boundary, intersecting_items);
        } else {
            low_child_->getIntersectingItems (boundary, intersecting_items);
            high_child_->getIntersectingItems (boundary, intersecting_items);
        }
    }
}

template <class T, int DIMENSIONS>
void Node<T, DIMENSIONS>::Divide () {
    assert (!has_children_);
    has_children_ = true;

    int dimension = depth_ % DIMENSIONS;
    // TODO: Test other strategies for dividing space
    division_boundary_ = (  this->min_coordinates_[dimension]
                          + this->max_coordinates_[dimension]) / 2;

    std::array<Coordinate, DIMENSIONS> new_min_coordinates,
                                       new_max_coordinates;
    for (int k = 0; k < DIMENSIONS; ++k) {
        if (k == dimension) {
            new_min_coordinates[k] = new_max_coordinates[k]
                = division_boundary_;
        } else {
            new_min_coordinates[k] = this->min_coordinates_[k];
            new_max_coordinates[k] = this->max_coordinates_[k];
        }
    }
    low_child_  = new Node(tree_, this, depth_ + 1, this->min_coordinates_, new_max_coordinates);
    high_child_ = new Node(tree_, this, depth_ + 1, new_min_coordinates, this->max_coordinates_);

    std::list<Item<T, DIMENSIONS> *> old_items_list = items_;
    items_.clear();
    for (Item<T, DIMENSIONS>* it : old_items_list)
        InsertItem(it);
}

template <class T, int DIMENSIONS>
void Node<T, DIMENSIONS>::Merge () {
    assert (has_children_);
    if (   !low_child_->has_children_
        && !high_child_->has_children_
         && (items_.size ()
             + low_child_->items_.size ()
             + high_child_->items_.size ()
               < tree_->max_elements_per_leaf () )) {
        for (Item<T, DIMENSIONS>* it : low_child_->items_) {
            items_.push_back(it);
            it->set_container_node(this);
        }
        for (Item<T, DIMENSIONS>* it : high_child_->items_) {
            items_.push_back(it);
            it->set_container_node(this);
        }
        delete low_child_;
        low_child_ = nullptr;
        delete high_child_;
        high_child_ = nullptr;
        has_children_ = false;
        if (parent_)
            parent_->Merge();
    }
}

template <class T, int DIMENSIONS>
void Node<T, DIMENSIONS>::Clear () {
    if (has_children_) {
        low_child_->Clear ();
        high_child_->Clear ();
        delete low_child_;
        low_child_ = nullptr;
        delete high_child_;
        high_child_ = nullptr;
    }
    for (Item<T, DIMENSIONS>* it : items_)
        delete it;
    items_.clear();
}

#ifdef DEBUG
template <class T, int DIMENSIONS>
void Node<T, DIMENSIONS>::PrintTreeRootedAtThisNode () {
    if (has_children_) {
        high_child_->PrintTreeRootedAtThisNode ();
    }
    for (int i = 0; i < depth_; ++i) {
        std::cout << "\t";
    }
    int i = 0;
    for (auto it = items_.begin(); it != items_.end(); ++it, ++i) {
        std::cout << "[" << i << "] = ";
        (*it)->Print ();
        std::cout << "; ";
    }
    std::cout << std::endl;
    if (has_children_) {
        low_child_->PrintTreeRootedAtThisNode ();
    }
}
#endif

} // namespace ikdtree
} // namespace structure
} // namespace ugdk

#endif
