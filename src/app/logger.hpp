#pragma once

#include <cstdarg>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

class Logger {
public:
  enum class LogLevel {
    LogLevel_Debug,
    LogLevel_Info,
    LogLevel_Warning,
    LogLevel_Error
  };

private:
  std::string m_log_file_path;
  std::vector<std::string> m_logs;

  static Logger *logger;
  static std::mutex mtx;

  // 设置日志输出文件
  explicit Logger(const std::string &log_file_path = "")
      : m_log_file_path(log_file_path),
        log_file(log_file_path.empty()
                     ? nullptr
                     : new std::ofstream(log_file_path, std::ios::app)) {}

public:
  static void init(const std::string &log_file_path) {
    if (logger == nullptr) {
      std::lock_guard<std::mutex> lock(mtx);
      if (logger == nullptr) {
        logger = new Logger(log_file_path);
      }
    }
  }

  static std::string get_logs_text() {
    std::lock_guard<std::mutex> lock(logger->log_mutex);
    if (logger != nullptr) {
      std::string text;
      size_t n_lines = logger->m_logs.size();

      for (int i = 0; i < n_lines; i++) {
        if (i == 0) {
          text += logger->m_logs[i];
        } else {
          text += '\n' + logger->m_logs[i];
        }
      }
      return text;
    }

    return "";
  }

  static std::vector<std::string> *get_logs_pointer() {
    if (logger != nullptr) {
      ::printf("%p\n", &(logger->m_logs));
      return &(logger->m_logs);
    }
    return nullptr;
  }

  static void close() {
    if (logger != nullptr) {
      delete logger;
      logger = nullptr;
    }
  }

  ~Logger() {
    if (log_file) {
      log_file->close();
      delete log_file;
    }
  }

  // 设置最低日志级别
  void set_log_level(LogLevel level) { min_level = level; }

  // 添加日志
  void log(LogLevel level, const std::string &message) {
    if (level < min_level)
      return;

    std::ostringstream log_entry;
    log_entry << "[" << get_current_time() << "] "
              << "[" << lvl_to_string(level) << "] " << message;

    std::lock_guard<std::mutex> lock(log_mutex);
    // 输出到控制台
    std::cout << log_entry.str() << std::endl;
    m_logs.push_back(log_entry.str());

    // 输出到文件
    if (log_file && log_file->is_open()) {
      *log_file << log_entry.str() << std::endl;
    }

    log_file->flush();
  }

  static std::string get_log_file_path() {
    if (logger != nullptr) {
      return logger->m_log_file_path;
    }

    return "";
  }

  static void clean_logs() {
    if (logger != nullptr) {
        logger->m_logs.clear();
    }
  }

  static void debug(const char *fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    buf[sizeof(buf) - 1] = '\0';
    debug(std::string(buf));
  }

  static void info(const char *fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    buf[sizeof(buf) - 1] = '\0';
    info(std::string(buf));
  }

  static void warning(const char *fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    buf[sizeof(buf) - 1] = '\0';
    warning(std::string(buf));
  }

  static void error(const char *fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    buf[sizeof(buf) - 1] = '\0';
    error(std::string(buf));
  }

  static void debug(const std::string &message) {
    if (logger != nullptr) {
      logger->log(LogLevel::LogLevel_Debug, message);
    }
  }
  static void info(const std::string &message) {
    if (logger != nullptr) {
      logger->log(LogLevel::LogLevel_Info, message);
    }
  }
  static void warning(const std::string &message) {
    if (logger != nullptr) {
      logger->log(LogLevel::LogLevel_Warning, message);
    }
  }
  static void error(const std::string &message) {
    if (logger != nullptr) {
      logger->log(LogLevel::LogLevel_Error, message);
    }
  }

private:
  LogLevel min_level = LogLevel::LogLevel_Debug; // 默认最低日志级别
  std::ofstream *log_file = nullptr;             // 日志文件
  std::mutex log_mutex;                          // 线程安全的锁

  // 获取当前时间字符串
  std::string get_current_time() {
    std::time_t now = std::time(nullptr);
    std::tm tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
  }

  // 日志级别转字符串
  std::string lvl_to_string(LogLevel level) {
    switch (level) {
    case LogLevel::LogLevel_Debug:
      return "DEBUG";
    case LogLevel::LogLevel_Info:
      return "INFO";
    case LogLevel::LogLevel_Warning:
      return "WARNING";
    case LogLevel::LogLevel_Error:
      return "ERROR";
    default:
      return "UNKNOWN";
    }
  }
};
