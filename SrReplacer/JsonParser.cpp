#include "JsonParser.h"

#include <fstream>
#include <algorithm>

#include "DebugLog.h"

bool JsonParser::loadJson(const std::string& file)
{
    try
    {
        std::ifstream metainfoStream(file, std::ifstream::binary);
        metainfoStream >> m_root;
    }
    catch (std::exception& e)
    {
        PRINT_ERROR("load file:", file, e.what());
        return false;
    }

    return true;
}

TMeasurements JsonParser::getMeasurments()
{
    TMeasurements measurements;
    auto jsonMeasurementItems = m_root["measurementItems"];

    if (jsonMeasurementItems.size() == 0)
    {
        PRINT_ERROR("No measurement items");
        return measurements;
    }

    for (uint32_t i = 0; i < jsonMeasurementItems.size(); i++)
    {
        auto &item = jsonMeasurementItems[i];

        Measurement measurement;
        measurement.Value = item["value"].asDouble();
        measurement.Description = item["measurmentDescription"].asCString();

        measurements.push_back(measurement);
    }

    return measurements;
}

std::vector<std::string> JsonParser::split(const std::string& str, char splitChar)
{
    std::vector<std::string> result;
    auto pos = str.find(splitChar);
    size_t initialPos = 0;

    while (pos != std::string::npos)
    {
        result.push_back(str.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;

        pos = str.find(splitChar, initialPos);
    }

    result.push_back(str.substr(initialPos, (std::min)(pos, str.size()) - initialPos + 1));

    return result;
}