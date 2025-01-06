#include <cstdarg>
#include "Log.h"

#define FORMAT_LOG_ENTRY(msg, formatted_msg) \
    char formatted_msg[4096]; \
    va_list ap; \
    va_start(ap, msg); \
    vsnprintf(formatted_msg, sizeof(formatted_msg), msg, ap); \
    va_end(ap);

void Log::Trace(const char* msg, ...)
{
    spdlog::set_level(spdlog::level::trace);
    FORMAT_LOG_ENTRY(msg, formattedMsg);
    spdlog::trace(formattedMsg);
}

void Log::Info(const char* msg, ...)
{
    spdlog::set_level(spdlog::level::info);
    FORMAT_LOG_ENTRY(msg, formattedMsg);
    spdlog::info(formattedMsg);
}

void Log::Warning(const char* msg, ...)
{
    spdlog::set_level(spdlog::level::warn);
    FORMAT_LOG_ENTRY(msg, formattedMsg);
    spdlog::warn(formattedMsg);
}

void Log::Critical(const char* msg, ...)
{
    spdlog::set_level(spdlog::level::critical);
    FORMAT_LOG_ENTRY(msg, formattedMsg);
    spdlog::critical(formattedMsg);
    exit(0);
}
