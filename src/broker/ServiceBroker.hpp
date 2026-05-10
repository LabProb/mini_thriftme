#pragma once

#include "services/VehicleService.hpp"

#include <string>

class ServiceBroker
{
public:
    std::string dispatch(
        const std::string& request);

private:
    VehicleService m_vehicleService;
};
