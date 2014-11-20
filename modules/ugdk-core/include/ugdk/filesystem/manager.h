#ifndef UGDK_FILESYSTEM_MANAGER_H_
#define UGDK_FILESYSTEM_MANAGER_H_

#include <ugdk/filesystem.h>

namespace ugdk {
namespace filesystem {

class Manager {
  public:
    Manager();
    ~Manager();

    bool Initialize();
    void Release();

  private:
};

}  // namespace filesystem
}  // namespace ugdk

#endif // UGDK_FILESYSTEM_MANAGER_H_
