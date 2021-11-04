#pragma once

#include <string>
#include <vector>

#include "json/json.h"

#include "Measurement.h"

class JsonParser
{
public:

    bool loadJson(const std::string& file);

    TMeasurements getMeasurments();

private:
    std::vector<std::string> split(const std::string& str, char splitChar);

    Json::Value m_root;
};
