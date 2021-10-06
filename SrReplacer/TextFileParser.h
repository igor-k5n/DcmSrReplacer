#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

class TextFileParser
{
public:

    using TPolylines = std::unordered_map<std::string, std::vector<float>>;

    bool load(const std::string& file);

    TPolylines getPolyline();

private:

    std::ifstream m_file;

    std::vector<std::string> split(const std::string& str, char splitChar);
};
