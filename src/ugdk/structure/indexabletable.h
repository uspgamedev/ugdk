#ifndef UGDK_STRUCTURE_INDEXABLETABLE_H_
#define UGDK_STRUCTURE_INDEXABLETABLE_H_

#include <ugdk/util/idgenerator.h>

#include <string>
#include <vector>
#include <unordered_map>

namespace ugdk {
namespace structure {

template<class T>
class IndexableTable { private: IndexableTable() {} };

template<class T>
class IndexableTable<T*> {
  public:
    static const int MAX_NUM_INDEX = 1024;

    IndexableTable(int expected_size = 16) : index_generator_(0, MAX_NUM_INDEX - 1, -1) {
        indexes_.reserve(expected_size);
    }
    ~IndexableTable() {
        for(auto& entry : data_)
            if(entry.second) delete entry.second;
    }

    size_t size() const {
        return data_.size();
    }

    void Add(const std::string& name, T* element) { data_[name] = element; }

    /** @return False if element don't exist, true otherwise. */
    bool Remove(const std::string& name) {
        auto it = data_.find(name);
        if(it == data_.end()) return false;
        T* val = it->second;
        data_.erase(it);
        removeIndexes(val);
        if(val) delete val;
        return true;
    }

    /// Searches for the element with the given name.
    T* Search(const std::string& name) const { 
        auto it = data_.find(name);
        return (it != data_.end()) ? it->second : nullptr;
    }
    
    /// Instant access to an element, using a pre-generated ID.
    T* Get(int index) const { return indexes_[index]; }

    // Optimizes access to the animations identified by the given name.
    // The caller should be conscious of the returned indexes for later use
    // of these animations through the Get() method.
    // Returns the generated index or -1 if the animation was not found.
    /// Generates an ID for future instant access.
    int MakeIndex(const std::string& name) {
        T* val = Search(name);
        if(!val) return index_generator_.error_value();
        int id = index_generator_.GenerateID();
        if(id != index_generator_.error_value()) {
            if(static_cast<typename decltype(indexes_)::size_type>(id) >= indexes_.size())
                indexes_.resize(id + 1);
            indexes_[id] = val;
        }
        return id;
    }

    /** 
    @arg id Id to be released.
    @return True on successful release, false otherwise. */
    bool ReleaseIndex(int id) {
        return index_generator_.ReleaseID(id) == id;
    }

  private:
    void removeIndexes(T* val) {
        for(int id = 0; id < static_cast<int>(indexes_.size()); ++id)
            if(indexes_[id] == val) {
                indexes_[id] = nullptr;
                index_generator_.ReleaseID(id);
            }
    }

    util::IDGenerator                   index_generator_;
    std::unordered_map<std::string, T*> data_;
    std::vector<T*>                     indexes_;
};

} // namespace structure
} // namespace ugdk

#endif // UGDK_STRUCTURE_INDEXABLETABLE_H_
