#include <sstream>
#include <iostream>
#include <cstdio>
#include <ugdk/util/pathmanager.h>

using namespace std;

namespace ugdk {

std::string PathManager::ResolvePath(const std::string& path) {
    std::ostringstream stm;
    stm << base_path_ << path;
	return stm.str();
}

} // namespace ugdk
