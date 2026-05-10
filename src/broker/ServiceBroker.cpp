#include "ServiceBroker.hpp"

std::string ServiceBroker::dispatch(
    const std::string& request)
{
    if (request == "GetVehicleSpeed")
    {
        return std::to_string(
            m_vehicleService.GetVehicleSpeed());
    }

    if (request == "GetCpuUsage")
    {
        return std::to_string(
            m_vehicleService.GetCpuUsage());
    }

    if (request == "GetMemoryUsage")
    {
        return std::to_string(
            m_vehicleService.GetMemoryUsage());
    }

    return "Unknown method";
}
