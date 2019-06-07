//
// Created by Jesus Lopez on 4/19/19.
//

#ifndef OBLIVIATE_LOGGER_H
#define OBLIVIATE_LOGGER_H


#include <cstdarg>
#include <android/log.h>

class Logger {
public:
    explicit Logger(const char *tag) : tag(tag) {}

    void log(android_LogPriority priority, const char *fmt, va_list ap) const;
    void debug(const char *fmt, ...) const __attribute__((__format__(printf, 2, 0)));
    void info(const char *fmt, ...) const __attribute__((__format__(printf, 2, 0)));
    void warn(const char *fmt, ...) const __attribute__((__format__(printf, 2, 0)));
    void error(const char *fmt, ...) const __attribute__((__format__(printf, 2, 0)));

private:
    const char *tag;
};

#endif //OBLIVIATE_LOGGER_H
