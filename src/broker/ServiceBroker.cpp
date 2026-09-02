#include "ServiceBroker.hpp"

DispatchResult ServiceBroker::dispatch(std::string_view method) const
{
    if (method == "GetVehicleSpeed")
    {
        return {true, std::to_string(m_vehicleService.GetVehicleSpeed())};
    }

    if (method == "GetCpuUsage")
    {
        return {true, std::to_string(m_vehicleService.GetCpuUsage())};
    }

    if (method == "GetMemoryUsage")
    {
        return {true, std::to_string(m_vehicleService.GetMemoryUsage())};
    }

    return {false, "ERROR: Unknown method"};
}
