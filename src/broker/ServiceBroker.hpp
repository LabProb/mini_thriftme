#pragma once

#include "common/RpcMessage.hpp"
#include "services/VehicleService.hpp"

class ServiceBroker
{
public:
    RpcMessage dispatch(const RpcMessage& request);

private:
    VehicleService m_vehicleService;
};
