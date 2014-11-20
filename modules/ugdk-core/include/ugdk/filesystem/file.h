#ifndef UGDK_FILESYSTEM_FILE_H_
#define UGDK_FILESYSTEM_FILE_H_

#include <ugdk/filesystem.h>

namespace ugdk {
namespace filesystem {

class File {
  public:
    virtual ~File() {}

  private:
    File() {}    
};

}  // namespace filesystem
}  // namespace ugdk

#endif  // UGDK_FILESYSTEM_FILE_H_
