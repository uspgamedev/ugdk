#ifndef HORUSEYE_FRAMEWORK_IDGENERATOR_H_
#define HORUSEYE_FRAMEWORK_IDGENERATOR_H_

#include <unordered_set>

#include <ugdk/util/uncopyable.h>

namespace ugdk {
namespace util {
class IdGenerator : public Uncopyable {
  public:
	  IdGenerator(int min_id, int max_id, int error_value);
    ~IdGenerator() {}
    /// Generate a new id.
    /** @return The new unique id. */
    int Id();
    /// Release an id.
    /** @return An error value or the recieve id.*/
    int RemoveId(int id);
  private:
    int                     current_id_;
    const int               min_id_,
                            max_id_,
                            error_value_;
    std::unordered_set<int> unused_ids_;
};

} // namespace util
} // namespace ugdk

#endif
