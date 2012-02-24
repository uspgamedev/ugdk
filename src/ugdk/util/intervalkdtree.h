#ifndef UGDK_UTIL_INTERVALKDTREE_H_
#define UGDK_UTIL_INTERVALKDTREE_H_

#include <cassert>
#include <map>
#include <list>
#include <vector>
#include <climits>
#ifdef DEBUG
#include <iostream>
#endif

namespace ugdk {

namespace ikdtree {

template <int DIMENSIONS>
class Box;

template <class T, int DIMENSIONS>
class Node;

template <class T, int DIMENSIONS>
class Item;

typedef double Coordinate;
#define COORD_NEG_INFINITY -11
#define COORD_INFINITY 11

template <class T, int DIMENSIONS>
class IntervalKDTree {
    public:
        IntervalKDTree (unsigned int max_elements_per_leaf);
        ~IntervalKDTree ();
        void Clear ();
        void Insert (Box<DIMENSIONS> bounding_box, T element);
        void Remove (T element);
        void Update (const Box<DIMENSIONS>& new_bounding_box, T element);
        std::vector<T>* getIntersectingItems (const Box<DIMENSIONS>& boundary) const;
        unsigned int max_elements_per_leaf ();
#ifdef DEBUG
        unsigned int max_height_;
        void PrintTree ();
#endif
    private:
        unsigned int max_elements_per_leaf_;
        std::map<T,Item<T, DIMENSIONS>* > container_items_;
        Node<T, DIMENSIONS> * root_;
        void UpdateItem (Item<T, DIMENSIONS> * item);
};

template <int DIMENSIONS>
class Box {
    public:
        Box (Coordinate *min_coordinates, Coordinate *max_coordinates);
        Box (const Box& box);
        void setBox (const Box& box);
        ~Box ();
        bool IsBelow (int depth, Coordinate boundary) const;
        bool IsAbove (int depth, Coordinate boundary) const;
        bool Contains (const Box& box) const;
        bool Intersects (const Box *box) const;
    protected:
        Coordinate min_coordinates_[DIMENSIONS];
        Coordinate max_coordinates_[DIMENSIONS];
};

//************* Implementation **************

//**** Classes needed for implementation ****

template <class T, int DIMENSIONS>
class Item : public Box<DIMENSIONS> {
    public :
        Item (Box<DIMENSIONS> bounding_box, T element);
        ~Item ();
        T element ();
        void set_container_node (Node<T, DIMENSIONS> *container_node);
        Node<T, DIMENSIONS> *container_node ();
#ifdef DEBUG
        void Print ();
#endif
    private:
        typedef Box<DIMENSIONS> super;
        T element_;
        Node<T, DIMENSIONS> *container_node_;
};

template <class T, int DIMENSIONS>
class Node : public Box<DIMENSIONS> {
    public:
        Node (IntervalKDTree<T, DIMENSIONS> * const tree, Node *parent, int depth,
                Coordinate *min_coordinates, Coordinate *max_coordinates);
        ~Node ();
        void InsertItem (Item<T, DIMENSIONS> *item);
        void RemoveItem (Item<T, DIMENSIONS> *item);
        void getIntersectingItems (const Box<DIMENSIONS>& boundary,
                std::vector<T> *items) const;
        int depth_;
        Coordinate division_boundary_;
        bool has_children_;
#ifdef DEBUG
        void PrintTreeRootedAtThisNode ();
#endif
    private:
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

        void Divide ();
        void Merge ();
};

// IntervalKDTree implementation

template <class T, int DIMENSIONS>
IntervalKDTree<T, DIMENSIONS>::IntervalKDTree (
                                        unsigned int max_elements_per_leaf) :
    max_elements_per_leaf_(max_elements_per_leaf), container_items_() {
    Coordinate min_coordinates[DIMENSIONS], max_coordinates[DIMENSIONS];
    for (int k = 0; k < DIMENSIONS; ++k) {
        min_coordinates[k] = COORD_NEG_INFINITY;
        max_coordinates[k] = COORD_INFINITY;
    }
    root_ = new Node<T,DIMENSIONS>(this, NULL, 0,
            min_coordinates, max_coordinates);
#ifdef DEBUG
    max_height_ = 0;
#endif
}

template <class T, int DIMENSIONS>
IntervalKDTree<T, DIMENSIONS>::~IntervalKDTree () {
    delete root_;
}

template <class T, int DIMENSIONS>
void IntervalKDTree<T, DIMENSIONS>::Clear () {
    container_items_.clear ();
    delete root_;
    Coordinate min_coordinates[DIMENSIONS], max_coordinates[DIMENSIONS];
    for (int k = 0; k < DIMENSIONS; ++k) {
        min_coordinates[k] = COORD_NEG_INFINITY;
        max_coordinates[k] = COORD_INFINITY;
    }
    root_ = new Node<T,DIMENSIONS>(this, NULL, 0,
            min_coordinates, max_coordinates);
}

template <class T, int DIMENSIONS>
void IntervalKDTree<T, DIMENSIONS>::Insert (Box<DIMENSIONS> bounding_box, T element) {
    // Shouldn't insert an element twice!
    assert (!container_items_.count (element));
    Item<T, DIMENSIONS> * item = new Item<T, DIMENSIONS> (bounding_box, element);
    root_->InsertItem (item);
    container_items_[element] = item;
}

template <class T, int DIMENSIONS>
void IntervalKDTree<T, DIMENSIONS>::Remove (T element) {
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
        T element) {
    // Shouldn't update an unexisting element!
    assert (container_items_.count(element));
    Item<T, DIMENSIONS> * item = container_items_[element];
    item->setBox (new_bounding_box);
    UpdateItem (item);
}

template <class T, int DIMENSIONS>
void IntervalKDTree<T, DIMENSIONS>::UpdateItem (Item<T, DIMENSIONS> * item) {
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
unsigned int IntervalKDTree<T, DIMENSIONS>::max_elements_per_leaf () {
    return max_elements_per_leaf_;
}

#ifdef DEBUG
template <class T, int DIMENSIONS>
void IntervalKDTree<T, DIMENSIONS>::PrintTree () {
    root_->PrintTreeRootedAtThisNode ();
}
#endif

// Box implementation

template <int DIMENSIONS>
Box<DIMENSIONS>::Box (Coordinate *min_coordinates, Coordinate *max_coordinates) {
    if (min_coordinates != NULL && max_coordinates != NULL) {
        for (int k = 0; k < DIMENSIONS; ++k) {
            min_coordinates_[k] = min_coordinates[k];
            max_coordinates_[k] = max_coordinates[k];
        }
    }
}

template <int DIMENSIONS>
Box<DIMENSIONS>::Box (const Box& box) {
    for (int k = 0; k < DIMENSIONS; ++k) {
        min_coordinates_[k] = box.min_coordinates_[k];
        max_coordinates_[k] = box.max_coordinates_[k];
    }
}

template <int DIMENSIONS>
Box<DIMENSIONS>::~Box () {}

template <int DIMENSIONS>
void Box<DIMENSIONS>::setBox (const Box& box) {
    for (int k = 0; k < DIMENSIONS; ++k) {
        min_coordinates_[k] = box.min_coordinates_[k];
        max_coordinates_[k] = box.max_coordinates_[k];
    }
}

template <int DIMENSIONS>
bool Box<DIMENSIONS>::IsBelow (int depth, Coordinate boundary) const {
    return max_coordinates_[depth % DIMENSIONS] <= boundary;
}

template <int DIMENSIONS>
bool Box<DIMENSIONS>::IsAbove (int depth, Coordinate boundary) const {
    return boundary < min_coordinates_[depth % DIMENSIONS];
}

template <int DIMENSIONS>
bool Box<DIMENSIONS>::Contains (const Box& box) const {
    for (int k = 0; k < DIMENSIONS; ++k) {
        // first comparison is a strict inequality
        // because of the way IsBelow and IsAbove are
        // defined.
        if (!(       min_coordinates_[k] <  box.min_coordinates_[k]
              && box.max_coordinates_[k] <=     max_coordinates_[k]))
            return false;
    }
    return true;
}

template <int DIMENSIONS>
bool Box<DIMENSIONS>::Intersects (const Box *box) const {
    for (int k = 0; k < DIMENSIONS; ++k) {
        if (!(        min_coordinates_[k] <= box->max_coordinates_[k]
              && box->min_coordinates_[k] <=      max_coordinates_[k]))
            // found separating axis
            return false;
    }
    return true;
}

// Item Implementation

template <class T, int DIMENSIONS>
Item<T, DIMENSIONS>::Item (Box<DIMENSIONS> bounding_box, T element) :
    super (bounding_box), element_(element) {}

template <class T, int DIMENSIONS>
Item<T, DIMENSIONS>::~Item () {}

template <class T, int DIMENSIONS>
Node<T, DIMENSIONS>* Item<T, DIMENSIONS>::container_node () {
    return container_node_;
}

template <class T, int DIMENSIONS>
void Item<T, DIMENSIONS>::set_container_node (Node<T, DIMENSIONS> *container_node) {
    container_node_ = container_node;
}

template <class T, int DIMENSIONS>
T Item<T, DIMENSIONS>::element () {
    return element_;
}

#ifdef DEBUG
template <class T, int DIMENSIONS>
void Item<T, DIMENSIONS>::Print () {
    std::cout << element_;
}
#endif

// Node Implementation

template <class T, int DIMENSIONS>
Node<T, DIMENSIONS>::Node (IntervalKDTree<T, DIMENSIONS> * const tree,
    Node *parent, int depth,
    Coordinate *min_coordinates, Coordinate *max_coordinates) :
    super (min_coordinates, max_coordinates), depth_(depth), has_children_(false), 
    tree_(tree), parent_(parent), low_child_(NULL), high_child_(NULL) {}

template <class T, int DIMENSIONS>
Node<T, DIMENSIONS>::~Node () {
    if (low_child_)
        delete low_child_;
    if (high_child_)
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

    Coordinate new_min_coordinates[DIMENSIONS],
               new_max_coordinates[DIMENSIONS];
    for (int k = 0; k < DIMENSIONS; ++k) {
        if (k == dimension) {
            new_min_coordinates[k] = new_max_coordinates[k]
                = division_boundary_;
        } else {
            new_min_coordinates[k] = this->min_coordinates_[k];
            new_max_coordinates[k] = this->max_coordinates_[k];
        }
    }
    low_child_ = new Node (tree_, this, depth_ + 1,
            this->min_coordinates_, new_max_coordinates);
    high_child_ = new Node (tree_, this, depth_ + 1,
            new_min_coordinates, this->max_coordinates_);

    std::list<Item<T, DIMENSIONS> *> old_items_list = items_;
    items_.clear();
    for (typename std::list<Item<T, DIMENSIONS> *>::iterator it
            = old_items_list.begin(); it != old_items_list.end(); ++it) {
        InsertItem (*it);
    }
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
        for (typename std::list<Item<T, DIMENSIONS> *>::iterator it
                = low_child_->items_.begin(); it != low_child_->items_.end();
                ++it) {
            items_.push_back (*it);
            (*it)->set_container_node (this);
        }
        for (typename std::list<Item<T, DIMENSIONS> *>::iterator it
                = high_child_->items_.begin(); it != high_child_->items_.end();
                ++it) {
            items_.push_back (*it);
            (*it)->set_container_node (this);
        }
        delete low_child_;
        low_child_ = NULL;
        delete high_child_;
        high_child_ = NULL;
        has_children_ = false;
        if (parent_)
            parent_->Merge();
    }
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
    for (typename std::list<Item<T, DIMENSIONS> *>::iterator it
            = items_.begin(); it != items_.end(); ++it, ++i) {
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

} // namespace ugdk

#endif
