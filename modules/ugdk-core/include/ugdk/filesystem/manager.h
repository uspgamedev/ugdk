#ifndef UGDK_FILESYSTEM_MANAGER_H_
#define UGDK_FILESYSTEM_MANAGER_H_

#include <ugdk/filesystem.h>

#include <string>
#include <memory>
#include <list>

namespace ugdk {
namespace filesystem {

class Manager {
  public:
    Manager();
    ~Manager();

    bool Initialize();
    void Release();

    void AddSearchPath(const std::string&);

    std::unique_ptr<File> OpenFile(const std::string& path, const char* mode = "rb") const;

  private:
    std::list<std::string> search_paths_;
};

}  // namespace filesystem
}  // namespace ugdk

#endif // UGDK_FILESYSTEM_MANAGER_H_
