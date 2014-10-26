
#include <ugdk/util/idgenerator.h>

#include <ugdk/system/exceptions.h>

namespace ugdk {
namespace util {

using std::unordered_set;

IDGenerator::IDGenerator(int min_id, int max_id, int error_value)
    : current_id_(min_id)
    , min_id_(min_id)
    , max_id_(max_id)
    , error_value_(error_value)
{
    if (min_id > max_id) {
        throw system::BaseException("IDGenerator: min_id > max_id (%d > %d)", min_id, max_id);
    }
}


int IDGenerator::GenerateID() {
    if (!unused_ids_.empty()) {
        int ret = (*unused_ids_.begin());
        unused_ids_.erase(ret);
        return ret;
    }
    if (current_id_ > max_id_) return error_value_;
    return current_id_++;
}

int IDGenerator::ReleaseID(int id) {
    if (id < min_id_ || id >= current_id_) return error_value_;
    if (unused_ids_.size() == range()) return error_value_;
    if (unused_ids_.insert(id).second == false) return error_value_;
    return id;
}

} // namespace util
} // namespace ugdk
