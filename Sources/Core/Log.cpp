#include "Log.h"
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace nkentseu {

#ifdef _WIN32
    inline void EnableANSIColors() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return;

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
#endif

    // Codes ANSI pour les couleurs
#ifdef _WIN32
    static constexpr const char* sColorDebug    = "\033[36m";    // Cyan
    static constexpr const char* sColorInfo     = "\033[32m";    // Vert
    static constexpr const char* sColorWarning  = "\033[33m";    // Jaune
    static constexpr const char* sColorError    = "\033[31m";    // Rouge
    static constexpr const char* sColorCritical = "\033[35m";    // Magenta
    static constexpr const char* sColorReset    = "\033[0m";     // Reset
#else
    static constexpr const char* sColorDebug    = "\033[36m";    // Cyan
    static constexpr const char* sColorInfo     = "\033[32m";    // Vert
    static constexpr const char* sColorWarning  = "\033[33m";    // Jaune
    static constexpr const char* sColorError    = "\033[31m";    // Rouge
    static constexpr const char* sColorCritical = "\033[35m";    // Magenta
    static constexpr const char* sColorReset    = "\033[0m";     // Reset
#endif

    std::unique_ptr<Logger> Logger::sInstance = nullptr;
    bool Logger::sInitialized = false;

    Logger::Logger() : mName("RihenNatural"), mLevel(LogLevel::Info) {
#ifdef _WIN32
        EnableANSIColors();
#endif
    }

    Logger& Logger::Instance() {
        if (!sInstance) {
            sInstance = std::make_unique<Logger>();
        }
        return *sInstance;
    }

    // Implémentation des nouvelles méthodes statiques
    void Logger::Initialize(const std::source_location& location, const std::string& timeString) {
        if (!sInitialized) {
            Instance(); // Force la création de l'instance
            sInitialized = true;
            Instance().Source(location, timeString).Info("Logger system initialized");
        }
    }

    void Logger::Shutdown(const std::source_location& location, const std::string& timeString) {
        if (sInitialized) {
            Instance().Source(location, timeString).Info("Logger system shutdown");
            sInstance.reset();
            sInitialized = false;
        }
    }

    void Logger::SetName(const std::string& name) {
        std::lock_guard lock(mMutex);
        mName = name;
    }

    void Logger::SetLevel(LogLevel level) {
        std::lock_guard lock(mMutex);
        mLevel = level;
    }

    Logger& Logger::Source(const std::source_location& location, const std::string& timeString) {
        std::lock_guard lock(mMutex);
        mCurrentLocation = location;
        mCurrentTime = timeString;
        return *this;
    }

    void Logger::Message(LogLevel level, const std::string& message) {
        if (level < mLevel) return;
        
        std::lock_guard lock(mMutex);
        
        auto timeStr = mCurrentTime;
        auto levelStr = GetLevelString(level);
        auto levelColor = GetLevelColor(level);
        auto resetColor = ResetColor();
        
        // Formater le nom de fichier pour être plus concis
        std::string filename = mCurrentLocation.file_name();
        size_t lastSlash = filename.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            filename = filename.substr(lastSlash + 1);
        }
        
        std::cout << "[" << mName << "] " << levelColor << "[" << levelStr << "] " << resetColor
                  << "[" << timeStr << "] " << filename << ":" << mCurrentLocation.line() << " - "
                  << mCurrentLocation.function_name() << " ->> " << message << std::endl;
    }

    std::string Logger::GetLevelString(LogLevel level) {
        switch (level) {
            case LogLevel::Debug:    return "DEBUG";
            case LogLevel::Info:     return "INFO";
            case LogLevel::Warning:  return "WARN";
            case LogLevel::Error:    return "ERROR";
            case LogLevel::Critical: return "CRITICAL";
            default:                 return "UNKNOWN";
        }
    }

    std::string Logger::GetLevelColor(LogLevel level) {
        switch (level) {
            case LogLevel::Debug:    return sColorDebug;
            case LogLevel::Info:     return sColorInfo;
            case LogLevel::Warning:  return sColorWarning;
            case LogLevel::Error:    return sColorError;
            case LogLevel::Critical: return sColorCritical;
            default:                 return "";
        }
    }

    std::string Logger::ResetColor() {
        return sColorReset;
    }

    std::string Logger::GetTimeString() {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_time), "%H:%M:%S")
           << '.' << std::setfill('0') << std::setw(3) << now_ms.count();
        
        return ss.str();
    }

    std::string ToString(const std::string& value) { return value; }
    std::string ToString(const char* value) { return value; }
    std::string ToString(bool value) { return value ? "true" : "false"; }

} // namespace nkentseu