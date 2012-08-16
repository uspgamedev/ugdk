#ifndef UGDK_ACTION_SPECIALIZEDANIMATIONMANAGER_H_
#define UGDK_ACTION_SPECIALIZEDANIMATIONMANAGER_H_

#include <ugdk/action/animationmanager.h>

#include <string>

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/util/indexabletable.h>


namespace ugdk {
namespace action {

template<class T>
class SpecializedAnimationManager : public AnimationManager {
public:
    SpecializedAnimationManager(const util::IndexableTable<T*> *table)
        : table_(table) {}

    ~SpecializedAnimationManager() {}

    const T* current() const { return dynamic_cast<const T*>(current_animation()); }
    const typename T::Frame* current_animation_frame() const {
        int index = current_animation_frame_index();
        if(index < 0 || index >= current_animation()->size()) return NULL;
        return current()->at(index);
    }

    /// Change the current animation to a new animation from the previously selected AnimationSet.
    /**Given a animation name (a string), the function changes the current animation to a new animation of AnimationSet*/
    void Select(const std::string& name) {
        set_current_animation(table_->Search(name));
    }

    /// Change the current animation to a new animation from the previo2usly selected AnimationSet.
    /**Given a animation index (a integer), the function changes the current animation to a new animation of AnimationSet*/
    void Select(int index) {
        set_current_animation(table_->Get(index));
    }

private:
    const util::IndexableTable<T*> *table_;
};

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_SPECIALIZEDANIMATIONMANAGER_H_ */


