#include <ugdk/util/idgenerator.h>

namespace ugdk {
namespace util {

using std::list;

int IdGenerator::Id() {
    if (!unused_ids_.empty()) {
        int ret = unused_ids_.front();
        unused_ids_.pop_front();
        return ret;
    }
    if (current_id_ > max_id_) return error_value_;
    return current_id_++;
}

int IdGenerator::RemoveId(int id) {
    if ( id < min_id_ || id > max_id_ ) return error_value_;
    if (unused_ids_.size() == ((max_id_ - min_id_) + 1) ) return error_value_;
    
    list<int>::const_iterator it;
    for ( it = unused_ids_.begin(); it != unused_ids_.end(); it++ ) {
        if ( *it == id ) return error_value_; 
    }
    unused_ids_.push_back(id);
    return id;
}

} // namespace util
} // namespace ugdk
