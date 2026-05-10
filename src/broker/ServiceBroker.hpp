#pragma once

#include "services/VehicleService.hpp"

#include <string>

class ServiceBroker
{
public:
    bool dispatch(
        const std::string& method,
        std::string& response);

private:
    VehicleService m_vehicleService;
};
