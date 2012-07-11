#include <ugdk/util/idgenerator.h>

namespace ugdk {
namespace util {

using std::unordered_set;

int IdGenerator::Id() {
    if (!unused_ids_.empty()) {
        int ret = (*unused_ids_.begin());
        unused_ids_.erase(ret);
        return ret;
    }
    if (current_id_ > max_id_) return error_value_;
    return current_id_++;
}

int IdGenerator::RemoveId(int id) {
    if (id < min_id_ || id > max_id_) return error_value_;
    if (unused_ids_.size() == ((max_id_ - min_id_) + 1)) return error_value_;
    if (unused_ids_.insert(id).second == false ) return error_value_;
    return id;
}

} // namespace util
} // namespace ugdk
