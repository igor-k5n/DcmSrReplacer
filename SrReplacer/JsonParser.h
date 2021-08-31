#pragma once

#include <string>
#include <vector>

#include "json/json.h"

class JsonParser
{
public:

    using TPolyline = std::vector<float>;

    bool loadJson(const std::string& file);

    TPolyline getPolyline();

private:
    std::vector<std::string> split(const std::string& str, char splitChar);

    Json::Value m_root;
};
