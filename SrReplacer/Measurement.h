#pragma once

#include <vector>
#include <string>

struct Measurement
{
    double Value;
    std::string Description;
};

using TMeasurements = std::vector<Measurement>;