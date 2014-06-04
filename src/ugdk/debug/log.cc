#include <ugdk/debug/log.h>
#include <ugdk/system/config.h>

#include <cstdio>

#ifdef ANDROID
#include <jni.h>
#include <string.h>
#include <android/log.h>
#endif

namespace ugdk {
namespace debug {

void RawLog(LogLevel level, const std::string& owner, const std::string& message) {
#ifdef ANDROID
    __android_log_print(ANDROID_LOG_INFO,owner.c_str(),message.c_str());
#else
    FILE* out = (level <= ERROR) ? stderr : stdout;
    fprintf(out, "%s %s - %s\n", owner.c_str(), ConvertLogToString(level), message.c_str());
#endif
}

}  // namespace debug
}  // namespace ugdk
