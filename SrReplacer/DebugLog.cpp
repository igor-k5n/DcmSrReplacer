#include "DebugLog.h"

#include <iostream>

DebugLog &DebugLog::getInstance()
{
    static DebugLog instance;
    return instance;
}

void DebugLog::print(const std::string &str)
{
    std::cout << str << std::endl;
}

