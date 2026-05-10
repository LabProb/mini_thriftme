#include "ServiceBroker.hpp"

RpcMessage ServiceBroker::dispatch(const RpcMessage& request)
{
    RpcMessage response;

    response.type = MessageType::Response;

    if (request.method == "GetVehicleSpeed")
    {
        response.payload =
            std::to_string(
                m_vehicleService.GetVehicleSpeed());
    }
    else if (request.method == "GetCpuUsage")
    {
        response.payload =
            std::to_string(
                m_vehicleService.GetCpuUsage());
    }
    else
    {
        response.payload = "Unknown method";
    }

    return response;
}
