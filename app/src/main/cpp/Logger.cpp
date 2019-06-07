//
// Created by Jesus Lopez on 4/19/19.
//

#include "Logger.h"

void Logger::debug(const char *fmt, ...) const {
    va_list ap;
    va_start(ap, fmt);
    log(ANDROID_LOG_DEBUG, fmt, ap);
    va_end(ap);
}

void Logger::info(const char *fmt, ...) const {
    va_list ap;
    va_start(ap, fmt);
    log(ANDROID_LOG_INFO, fmt, ap);
    va_end(ap);
}

void Logger::warn(const char *fmt, ...) const {
    va_list ap;
    va_start(ap, fmt);
    log(ANDROID_LOG_INFO, fmt, ap);
    va_end(ap);
}

void Logger::error(const char *fmt, ...) const {
    va_list ap;
    va_start(ap, fmt);
    log(ANDROID_LOG_ERROR, fmt, ap);
    va_end(ap);
}

void Logger::log(android_LogPriority priority, const char *fmt, va_list ap) const {
    __android_log_vprint(priority, tag, fmt, ap);
}
