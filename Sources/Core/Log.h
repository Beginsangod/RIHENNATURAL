#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <source_location>
#include <iostream>
#include <memory>
#include <mutex>

namespace nkentseu {
        
    // Conversion de types en string
    template<typename T>
    std::string ToString(const T& value) {
        if constexpr (std::is_arithmetic_v<T>) {
            return std::to_string(value);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return value;
        } else if constexpr (std::is_same_v<T, const char*>) {
            return std::string(value);
        } else if constexpr (std::is_same_v<T, bool>) {
            return value ? "true" : "false";
        } else {
            return std::to_string(value);
        }
    }
    
    std::string ToString(const std::string& value);
    std::string ToString(const char* value);
    std::string ToString(bool value);
    
    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    class Logger {
    public:
        static Logger& Instance();
        static void Initialize(const std::source_location& location = std::source_location::current(), const std::string& timeString = GetTimeString());    // Nouvelle méthode
        static void Shutdown(const std::source_location& location = std::source_location::current(), const std::string& timeString = GetTimeString());      // Nouvelle méthode

        Logger();
        ~Logger() = default;
        
        // Configuration
        void SetName(const std::string& name);
        void SetLevel(LogLevel level);
        
        // Méthodes de logging avec source location
        Logger& Source(const std::source_location& location = std::source_location::current(), const std::string& timeString = GetTimeString());
        
        void Message(LogLevel level, const std::string& message);
        
        template<typename... Args>
        void Debug(const std::string& format, Args&&... args) {
            Message(LogLevel::Debug, FormatString(format, std::forward<Args>(args)...));
        }
        
        template<typename... Args>
        void Info(const std::string& format, Args&&... args) {
            Message(LogLevel::Info, FormatString(format, std::forward<Args>(args)...));
        }
        
        template<typename... Args>
        void Warning(const std::string& format, Args&&... args) {
            Message(LogLevel::Warning, FormatString(format, std::forward<Args>(args)...));
        }
        
        template<typename... Args>
        void Error(const std::string& format, Args&&... args) {
            Message(LogLevel::Error, FormatString(format, std::forward<Args>(args)...));
        }
        
        template<typename... Args>
        void Critical(const std::string& format, Args&&... args) {
            Message(LogLevel::Critical, FormatString(format, std::forward<Args>(args)...));
        }
        
        // Méthodes print/println
        template<typename... Args>
        void Print(const std::string& format, Args&&... args) {
            std::cout << FormatString(format, std::forward<Args>(args)...);
        }
        
        template<typename... Args>
        void Println(const std::string& format, Args&&... args) {
            std::cout << FormatString(format, std::forward<Args>(args)...) << std::endl;
        }

    private:
        // Formatage
        
        // Formatage simplifié
        template<typename... Args>
        std::string FormatString(const std::string& format, Args&&... args) {
            std::string result = format;
            size_t index = 0;
            ((FormatReplace(result, index++, std::forward<Args>(args))), ...);
            return result;
        }
        
        template<typename T>
        void FormatReplace(std::string& str, size_t index, T&& value) {
            std::string placeholder = "{" + std::to_string(index) + "}";
            size_t pos = str.find(placeholder);
            if (pos != std::string::npos) {
                str.replace(pos, placeholder.length(), ToString(std::forward<T>(value)));
            }
        }
        
        // Utilitaires
        std::string GetLevelString(LogLevel level);
        std::string GetLevelColor(LogLevel level);
        static std::string GetTimeString();
        std::string ResetColor();
        
    private:
        std::string mName;
        std::string mCurrentTime;
        LogLevel mLevel;
        std::source_location mCurrentLocation;
        std::mutex mMutex;
        
        static std::unique_ptr<Logger> sInstance;
        static bool sInitialized;  // Nouveau membre statique
    };

    // Macros pour un usage simplifié
    #define Log Logger::Instance().Source()

} // namespace nkentseu