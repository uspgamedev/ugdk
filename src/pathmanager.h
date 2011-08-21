#ifndef HORUSEYE_FRAMEWORK_PATHMANAGER_H_
#define HORUSEYE_FRAMEWORK_PATHMANAGER_H_

#include <string>
#define PATH_MANAGER() ugdk::Engine::reference()->path_manager()

namespace ugdk {

class PathManager{
  public:
	PathManager(const std::string& base_path) : base_path_(base_path) {}
    ~PathManager() {}

	std::string ResolvePath(const std::string& path);
	std::string ResolvePath(const std::wstring& path);

  private:
	std::string base_path_;

};

} // namespace framework

#endif
