#include <cstdio>

#include "log.h"

namespace ugdk {
namespace log {

void Log(LogLevel level, const std::string& message) {
    FILE* out = (level < ERROR) ? stderr : stdout;
    fprintf(out, "%s - %s\n", ConvertLogToString(level), message.c_str());
}

void Log(LogLevel level, const std::string& owner, const std::string& message) {
    FILE* out = (level < ERROR) ? stderr : stdout;
    fprintf(out, "%s %s - %s\n", owner.c_str(), ConvertLogToString(level), message.c_str());
}

}  // namespace log
}  // namespace ugdk
