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

JsonParser::TPolyline JsonParser::getPolyline()
{
    auto jsonMeasurements = m_root["Measurements"];

    if (jsonMeasurements.size() == 0)
    {
        PRINT_ERROR("No measurements");
        return TPolyline();
    }

    if (jsonMeasurements.size() > 1)
    {
        PRINT_ERROR("Measurements more then 1");
        return TPolyline();
    }

    auto jsonMeasurementItems = jsonMeasurements[0]["measurementItems"];

    if (jsonMeasurementItems.size() == 0)
    {
        PRINT_ERROR("No measurement items");
        return TPolyline();
    }

    if (jsonMeasurementItems.size() > 1)
    {
        PRINT_ERROR("Measurement items more then 1");
        return TPolyline();
    }

    std::string polylineString = jsonMeasurementItems[0]["measurementPopulationDescription"].asCString();

    if (polylineString.empty())
    {
        PRINT_ERROR("measurement population description string is empty");
        return TPolyline();
    }

    auto pointsSplit = split(polylineString, ',');
    
    TPolyline result;
    std::transform(std::begin(pointsSplit), std::end(pointsSplit), std::back_inserter(result),
        [](auto &&value)
        {
            return std::stof(value);
        });

    return result;
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