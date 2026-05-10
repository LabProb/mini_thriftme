#pragma once

#include <string>

class VehicleService
{
public:
    int GetVehicleSpeed() const;

    double GetCpuUsage() const;

    double GetMemoryUsage() const;

    std::string GetUptime() const;
};
