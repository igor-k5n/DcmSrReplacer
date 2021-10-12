#include "TextFileParser.h"

#include <algorithm>

bool TextFileParser::load(const std::string& file)
{
    m_file.open(file);

    return m_file.is_open();
}

TextFileParser::TPolylines TextFileParser::getPolyline()
{
    int32_t index = 0;
    TPolylines result;
    while (!m_file.eof())
    {
        index++;

        std::string text;
        std::vector<float> polyline;

        std::string polylineString;
        std::getline(m_file, polylineString);
        if (!polylineString.empty())
        {
            auto polylineSplit = split(polylineString, ' ');

            std::transform(std::begin(polylineSplit), std::end(polylineSplit), std::back_inserter(polyline),
                [](const auto& value)
                {
                    return static_cast<float>(std::atof(value.c_str()));
                });

            text = std::to_string(index);
            if (!text.empty() && !polyline.empty())
            {
                result.emplace(std::move(text), std::move(polyline));
            }
        }
    }

    return result;
}

std::vector<std::string> TextFileParser::split(const std::string& str, char splitChar)
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

