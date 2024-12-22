#include <app/logger.hpp>

Logger *Logger::logger = nullptr;

std::mutex Logger::mtx;
