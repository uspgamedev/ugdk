#include <ugdk/debug/log.h>

#include <cstdio>

namespace ugdk {
namespace debug {

void Log(LogLevel level, const std::string& message) {
    Log(level, "UGDK", message);
}

void Log(LogLevel level, const std::string& owner, const std::string& message) {
    FILE* out = (level < ERROR) ? stderr : stdout;
    fprintf(out, "%s %s - %s\n", owner.c_str(), ConvertLogToString(level), message.c_str());
}

}  // namespace debug
}  // namespace ugdk
