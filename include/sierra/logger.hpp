// logger.hpp
// Licensed under LGPLv2.1

#pragma once

#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <sstream>

#ifndef ERRPRINTF
#define ERRPRINTF(...) \
    fprintf(stderr, "\033[31m"); \
    fprintf(stderr,  __VA_ARGS__); \
    fprintf(stderr, "\033[0m"); 
#endif

#if defined(__GNUC__) || defined(__clang__)
#define SIERRA_LOG_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define SIERRA_LOG_FUNCTION __FUNCSIG__
#else
#define SIERRA_LOG_FUNCTION __func__
#endif

namespace SierraEditor {
    struct LogEntry {
        enum class Level {
            INFO,
            WARNING,
            ERROR,
            DEBUG,
            TODO
        };

        Level level;
        std::string message;
        std::string functionName = "";
        std::string fileName = "";

        // This will get overriden, do not bother setting it manually
        int lineNumber;
    };

    class Logger {
        public:
            Logger() = default;
            Logger(const Logger&) = delete;
            Logger& operator=(const Logger&) = delete;
            Logger(Logger&&) = delete;
            Logger& operator=(Logger&&) = delete;
            ~Logger() = default;

            static Logger& getInstance() {
                static Logger instance;
                return instance;
            }

            static void addConsoleEntry(LogEntry& entry) {
                int num = getInstance().mConsoleEntries.size();
                Logger& logger = getInstance();
                entry.lineNumber = num + 1;
                logger.mConsoleEntries.push_back(entry);
            }

            static void addGameLogEntry(LogEntry& entry) {
                int num = getInstance().mGameLogEntries.size();
                Logger& logger = getInstance();
                entry.lineNumber = num + 1;
                logger.mGameLogEntries.push_back(entry);
            }

            static const std::vector<LogEntry>& getConsoleEntries() {
                return getInstance().mConsoleEntries;
            }

            static const std::vector<LogEntry>& getGameLogEntries() {
                return getInstance().mGameLogEntries;
            }
        
        private:
            std::vector<LogEntry> mConsoleEntries;
            std::vector<LogEntry> mGameLogEntries;
    };

    namespace detail {
        // Route macro logs into the console log store.
        inline void addConsoleLog(LogEntry::Level level, const char* functionName, const char* fileName, const std::string& message) {
            LogEntry entry{};
            entry.level = level;
            entry.message = message;
            if (functionName) {
                entry.functionName = functionName;
            }
            if (fileName) {
                entry.fileName = fileName;
            }
            entry.lineNumber = 0;
            Logger::addConsoleEntry(entry);
        }

        inline const char* levelLabel(LogEntry::Level level) {
            switch (level) {
                case LogEntry::Level::INFO: return "LOG";
                case LogEntry::Level::WARNING: return "WARN";
                case LogEntry::Level::ERROR: return "ERROR";
                case LogEntry::Level::DEBUG: return "DEBUG";
                case LogEntry::Level::TODO: return "TODO";
            }
            return "LOG";
        }

        inline const char* levelColor(LogEntry::Level level) {
            switch (level) {
                case LogEntry::Level::INFO: return "\033[0m";
                case LogEntry::Level::WARNING: return "\033[33m";
                case LogEntry::Level::ERROR: return "\033[31m";
                case LogEntry::Level::DEBUG: return "\033[95m";
                case LogEntry::Level::TODO: return "\033[32m";
            }
            return "\033[0m";
        }

        inline void printConsole(LogEntry::Level level, const char* functionName, const std::string& message) {
            std::cout << levelColor(level) << "(" << levelLabel(level) << ") " << "\033[0m";
            if (functionName) {
                std::cout << functionName << ": ";
            } else {
                std::cout << ": ";
            }
            std::cout << message << "\n";
        }
    }
}

#ifndef LOG
#define LOG(msg) \
    do { \
        std::ostringstream _se_log_stream; \
        _se_log_stream << msg; \
        SierraEditor::detail::printConsole(SierraEditor::LogEntry::Level::INFO, SIERRA_LOG_FUNCTION, _se_log_stream.str()); \
        SierraEditor::detail::addConsoleLog(SierraEditor::LogEntry::Level::INFO, SIERRA_LOG_FUNCTION, __FILE__, _se_log_stream.str()); \
    } while (0)
#endif

#ifndef WARN
#define WARN(msg) \
    do { \
        std::ostringstream _se_log_stream; \
        _se_log_stream << msg; \
        SierraEditor::detail::printConsole(SierraEditor::LogEntry::Level::WARNING, SIERRA_LOG_FUNCTION, _se_log_stream.str()); \
        SierraEditor::detail::addConsoleLog(SierraEditor::LogEntry::Level::WARNING, SIERRA_LOG_FUNCTION, __FILE__, _se_log_stream.str()); \
    } while (0)
#endif

#ifndef TODO
#define TODO(msg) \
    do { \
        std::ostringstream _se_log_stream; \
        _se_log_stream << msg; \
        SierraEditor::detail::printConsole(SierraEditor::LogEntry::Level::TODO, SIERRA_LOG_FUNCTION, _se_log_stream.str()); \
        SierraEditor::detail::addConsoleLog(SierraEditor::LogEntry::Level::TODO, SIERRA_LOG_FUNCTION, __FILE__, _se_log_stream.str()); \
    } while (0)
#endif

#ifndef ERROR
#define ERROR(msg) \
    do { \
        std::ostringstream _se_log_stream; \
        _se_log_stream << msg; \
        SierraEditor::detail::printConsole(SierraEditor::LogEntry::Level::ERROR, SIERRA_LOG_FUNCTION, _se_log_stream.str()); \
        SierraEditor::detail::addConsoleLog(SierraEditor::LogEntry::Level::ERROR, SIERRA_LOG_FUNCTION, __FILE__, _se_log_stream.str()); \
    } while (0)
#endif

#ifndef DBG
#define DBG(msg) \
    do { \
        std::ostringstream _se_log_stream; \
        _se_log_stream << #msg << ": " << msg; \
        SierraEditor::detail::printConsole(SierraEditor::LogEntry::Level::DEBUG, SIERRA_LOG_FUNCTION, _se_log_stream.str()); \
        SierraEditor::detail::addConsoleLog(SierraEditor::LogEntry::Level::DEBUG, SIERRA_LOG_FUNCTION, __FILE__, _se_log_stream.str()); \
    } while (0)
#endif