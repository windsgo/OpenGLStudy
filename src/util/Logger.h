#pragma once

#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <sstream>

#include <mutex>

namespace util {

class Logger {
private:
    Logger() {}
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;

    std::mutex _mutex;

    enum class LogLevel {
        INFO = 0, DEBUG, WARNING, ERROR
    };

    const std::array<std::string, 4> _log_color{
        "\033[0m", "\033[32m", "\033[33m", "\033[31m"
    };
    const std::array<std::string, 4> _log_text{
        "[INFO]", "[DEBUG]", "[WARNING]", "[ERROR]"
    };

    const std::string _log_bold{"\033[1m"};

    void _log(std::stringstream& ss) {
        ss << '\n';
    }

    template <typename T, typename ...Args>
    void _log(std::stringstream& ss, T&& first, Args&&... args) {
        ss << first;
        _log(ss, std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void log(LogLevel level, Args&&... args) {
        std::lock_guard locker(_mutex);

        std::stringstream ss;

        ss << _log_color[static_cast<int>(level)] << _log_bold << _log_text[static_cast<int>(level)] << _log_color[0] << _log_color[static_cast<int>(level)] << ' ';

        _log(ss, std::forward<Args>(args)...);

        ss << _log_color[0];

        std::cout << ss.str();
    }

public:
    inline static Logger* getInstance() {
        static Logger unique_instance;
        return &unique_instance; 
    }

    template <typename ...Args>
    void info(Args&&... args) {
        log(LogLevel::INFO, std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void debug(Args&&... args) {
        log(LogLevel::DEBUG, std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void warning(Args&&... args) {
        log(LogLevel::WARNING, std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void error(Args&&... args) {
        log(LogLevel::ERROR, std::forward<Args>(args)...);
    }

};

}