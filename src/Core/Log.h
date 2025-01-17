#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>
#include <mutex>

// TODO: Define log behavior as a singleton to
//       enable color output.

class Log
{
public:
    static void Trace(const char* msg, ...);
    static void Info(const char* msg, ...);
    static void Warning(const char* msg, ...);
    static void Critical(const char* msg, ...);

private:
    Log() = default;
    ~Log() = default;
};
