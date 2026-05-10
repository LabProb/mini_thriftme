#include "ServiceBroker.hpp"

bool ServiceBroker::dispatch(
    const std::string& method,
    std::string& response)
{
    bool result = true;
    if (method == "GetVehicleSpeed")
    {
        response = std::to_string(
            m_vehicleService.GetVehicleSpeed());
    }

    else if (method == "GetCpuUsage")
    {
        response = std::to_string(
            m_vehicleService.GetCpuUsage());
    }

    else if (method == "GetMemoryUsage")
    {
        response = std::to_string(
            m_vehicleService.GetMemoryUsage());
    }
    else {
        response = "ERROR: Unknown method";
        result = false;
    }
    return result;
}
