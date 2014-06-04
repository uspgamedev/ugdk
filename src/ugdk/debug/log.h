#ifndef UGDK_DEBUG_LOG_H_
#define UGDK_DEBUG_LOG_H_

#include <string>
#include <sstream>

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

void RawLog(LogLevel, const std::string& owner, const std::string& message);

#ifndef SWIG
inline void InsertToStream(std::stringstream& ss) {}

template<typename T, typename... Ts>
inline void InsertToStream(std::stringstream& ss, const T& t, Ts&&... ts) {
    ss << t;
    InsertToStream(ss, std::forward<Ts>(ts)...);
}

template<typename... Ts>
void Log(LogLevel level, const std::string& owner, Ts&&... ts) {
    std::stringstream ss;
    InsertToStream(ss, std::forward<Ts>(ts)...);
    RawLog(level, owner, ss.str());
}

template<typename... Ts>
inline void DebugLog(LogLevel level, const std::string& owner, Ts&&... ts) {
#ifndef NDEBUG
    Log(level, owner, std::forward<Ts>(ts)...);
#endif
}

#endif // SWIG

}  // namespace debug
}  // namespace ugdk

#endif // UGDK_DEBUG_LOG_H_
