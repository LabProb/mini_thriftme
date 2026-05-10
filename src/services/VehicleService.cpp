#include "VehicleService.hpp"

int VehicleService::GetVehicleSpeed() const
{
    return 72;
}

double VehicleService::GetCpuUsage() const
{
    return 31.5;
}

double VehicleService::GetMemoryUsage() const
{
    return 48.2;
}

std::string VehicleService::GetUptime() const
{
    return "5h 12m";
}
