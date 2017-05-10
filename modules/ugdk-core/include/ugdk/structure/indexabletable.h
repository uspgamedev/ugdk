#ifndef UGDK_STRUCTURE_INDEXABLETABLE_H_
#define UGDK_STRUCTURE_INDEXABLETABLE_H_

#include <ugdk/util/idgenerator.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace ugdk {
namespace structure {

template<class T>
class IndexableTable {
  public:
    static const int MAX_NUM_INDEX = 1024;

    IndexableTable(int expected_size = 16) : index_generator_(0, MAX_NUM_INDEX - 1, -1) {
        indices_.reserve(expected_size);
    }
    ~IndexableTable() {}

    size_t size() const {
        return data_.size();
    }

#ifndef SWIG
    void Add(const std::string& name, std::unique_ptr<T> element) {
        data_[name] = std::move(element);
    }
#endif

    /// Removes the element with the given name.
    /** @return False if element don't exist, true otherwise. */
    bool Remove(const std::string& name) {
        auto it = data_.find(name);
        if (it == data_.end()) {
            return false;
        }
        removeIndices(it->second.get());
        data_.erase(it);
        return true;
    }

    /// Searches for the element with the given name.
    T* Search(const std::string& name) const {
        auto it = data_.find(name);
        if (it == data_.end()) {
            return nullptr;
        }
        else {
            return it->second.get();
        }
    }

    /// Instant access to an element, using a pre-generated ID.
    T* Get(int index) const { return indices_[index]; }

    // Optimizes access to the animations identified by the given name.
    // The caller should be conscious of the returned indexes for later use
    // of these animations through the Get() method.
    // Returns the generated index or -1 if the animation was not found.
    /// Generates an ID for future instant access.
    int MakeIndex(const std::string& name) {
        T* val = Search(name);
        if (!val) {
            return index_generator_.error_value();
        }
        int id = index_generator_.GenerateID();
        if(id != index_generator_.error_value()) {
            if (static_cast<decltype(indices_.size())>(id) >= indices_.size()) {
                indices_.resize(id + 1);
            }
            indices_[id] = val;
        }
        return id;
    }

    /**
    @arg id Id to be released.
    @return True on successful release, false otherwise. */
    bool ReleaseIndex(int id) {
        return index_generator_.ReleaseID(id) == id;
    }

<<<<<<< HEAD
    // Allows to return iteratiors to the index vector
    class IndexIterable {
        public:
            
            IndexIterable() {
                index_vector_ = indices_;
            }

            ~IndexIterable() {}

            decltype(index_vector_.begin()) begin() const { 
                return index_vector_.begin(); 
            }

            decltype(index_vector_ = end()) end() const { 
                return index_vector_ = end();
            }

        private:
            std::vector<T*> & index_vector_; 

    }
=======
    //Begin end methods for the instance's data data_
    auto begin() {
        return unordered_map.begin();
    }
    auto end() {
        return unordered_map.end();
    }
    auto begin() const {
        return unordered_map.begin();
    }
    auto end() const {
        return unordered_map.end();
    }

>>>>>>> 6945587bdb56a23ebd570b0bef4db8f9fabaa0fe

  private:
    void removeIndices(T* val) {
        for (int id = 0; id < static_cast<int>(indices_.size()); ++id) {
            if (indices_[id] == val) {
                indices_[id] = nullptr;
                index_generator_.ReleaseID(id);
            }
        }
    }

    util::IDGenerator index_generator_;
    std::unordered_map<std::string, std::unique_ptr<T>> data_;
    std::vector<T*> indices_;
};

} // namespace structure
} // namespace ugdk

#endif // UGDK_STRUCTURE_INDEXABLETABLE_H_
