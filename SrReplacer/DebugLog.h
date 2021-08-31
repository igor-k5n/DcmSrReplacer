#pragma once

#include <string>
#include <sstream>
#include <type_traits>

class DebugLog
{

public:

    static DebugLog& getInstance();

    template<typename ... TArgs>
    static void printDebug(TArgs&& ... args)
    {
        std::ostringstream stream;
         ((void) (stream << std::forward<TArgs>(args) << " "), ...);

        getInstance().print(stream.str());
    }

private:

    void print(const std::string&);

    DebugLog() = default;
    ~DebugLog() = default;

};

#define PRINT_ERROR(...) DebugLog::printDebug(std::string("ERROR |"), std::string(__FUNCTION__), "|", __VA_ARGS__)
#define PRINT_VARIABLE_ERROR(x, ...) PRINT_ERROR(#x, __VA_ARGS__)
