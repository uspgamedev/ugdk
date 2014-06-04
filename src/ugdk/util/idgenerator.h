#ifndef HORUSEYE_FRAMEWORK_IDGENERATOR_H_
#define HORUSEYE_FRAMEWORK_IDGENERATOR_H_

#include <unordered_set>

#include <ugdk/util/uncopyable.h>

namespace ugdk {
namespace util {
class IDGenerator : public Uncopyable {
  public:
    IDGenerator(int min_id, int max_id, int error_value);
    ~IDGenerator() {}

    /// Generate a new id.
    /** @return The new unique id. */
    int GenerateID();

    /// Release an id.
    /** @return An error value or the recieved id.*/
    int ReleaseID(int id);

    /// The error value for this generator.
    int error_value() const { return error_value_; }

  private:
    int current_id_;
    const int min_id_,
              max_id_,
              error_value_;
    
    std::unordered_set<int> unused_ids_;
    std::size_t range() const {
        return static_cast<std::size_t>(max_id_ - min_id_ + 1);
    }
};

} // namespace util
} // namespace ugdk

#endif
