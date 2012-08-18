#ifndef UGDK_UTIL_INDEXABLETABLE_H_
#define UGDK_UTIL_INDEXABLETABLE_H_

#include <ugdk/portable/tr1.h>

#include <string>
#include <vector>
#include FROM_TR1(unordered_map)

#include <ugdk/util/idgenerator.h>

namespace ugdk {
namespace util {

template<class T, int IndexSize = 16>
class IndexableTable { private: IndexableTable() {} };

template<class T, int IndexSize>
class IndexableTable<T*, IndexSize> {
private:
    typedef typename std::tr1::unordered_map<std::string, T*> Table;
    typedef typename Table::iterator TableIterator;
    typedef typename Table::const_iterator TableConstIterator;

    typedef typename std::vector<T*> IndexArray;
  public:
    IndexableTable() : index_generator_(0, IndexSize - 1, -1), indexes_(IndexSize, static_cast<T*>(NULL)) {}
    ~IndexableTable() {
        for(TableIterator it = data_.begin(); it != data_.end(); ++it)
            if(it->second) delete it->second;
    }

    void Add(const std::string& name, T* element) { data_[name] = element; }

    /** @return False if element don't exist, true otherwise. */
    bool Remove(const std::string& name) {
        TableIterator it = data_.find(name);
        if(it == data_.end()) return false;
        T* val = it->second;
        data_.erase(it);
        removeIndexes(val);
        if(val) delete val;
        return true;
    }

    /// Searches for the element with the given name.
    T* Search(const std::string& name) const { 
        TableConstIterator it = data_.find(name);
        return (it != data_.end()) ? it->second : NULL;
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
        if(id != index_generator_.error_value())
            indexes_[id] = val;
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
                indexes_[id] = NULL;
                index_generator_.ReleaseID(id);
            }
    }

    IDGenerator index_generator_;
    Table data_;
    IndexArray indexes_;
};

} // namespace util
} // namespace ugdk

#endif // UGDK_UTIL_CACHEDSET_H_
