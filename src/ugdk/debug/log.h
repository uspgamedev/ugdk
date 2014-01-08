#ifndef UGDK_DEBUG_LOG_H_
#define UGDK_DEBUG_LOG_H_

#include <string>

namespace ugdk {
namespace debug {

/// These are the log levels.
#define LOG_LEVELS(ACTION)  \
    ACTION(EMERGENCY) \
    ACTION(ALERT) \
    ACTION(CRITICAL) \
    ACTION(ERROR) \
    ACTION(WARNING) \
    ACTION(NOTICE) \
    ACTION(INFO)
    //ACTION(DEBUG)

/// Through macro wizardy(tm), fill the LogLevel enum.    
#define LIST(X) X, 
enum LogLevel {
    LOG_LEVELS(LIST)
};
#undef LIST

// Even more Macro Wizardry(tm).
/** Converts LogLevels into strings. 
    @param level The target log level
    @return The string associated with the level */
#define CASE_LOG(X) case X: return #X;
inline const char* ConvertLogToString(LogLevel level) {
    switch(level) {
    LOG_LEVELS(CASE_LOG)
    default: return "";
    }
}
#undef CASE_LOG

void Log(LogLevel, const std::string& owner, const std::string& message);

void Log(LogLevel level, const std::string& message);

inline void DebugLog(LogLevel level, const std::string& message) {
#ifdef _DEBUG
    Log(level, message);
#endif
}

inline void DebugLog(LogLevel level, const std::string& owner, const std::string& message) {
#ifdef _DEBUG
    Log(level, owner, message);
#endif
}

}  // namespace debug
}  // namespace ugdk

#endif // UGDK_DEBUG_LOG_H_
