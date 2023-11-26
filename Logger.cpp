#include "Logger.h"

Logger::Logger(const std::string& filename) {
    file_.open(filename, std::ios::out | std::ios::app);
}

Logger::~Logger() {
    file_.close();
}

void Logger::log(const std::string& message) {
    file_ << message << std::endl;
}