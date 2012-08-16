#ifndef UGDK_ACTION_ANIMATIONSET_H_
#define UGDK_ACTION_ANIMATIONSET_H_

#include <ugdk/action.h>
#include <ugdk/util/indexabletable.h>

namespace ugdk {
namespace action {

typedef ::ugdk::util::IndexableTable<ugdk::action::Animation*> AnimationSet;

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_ANIMATIONSET_H_ */
