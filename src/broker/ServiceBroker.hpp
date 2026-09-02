#pragma once

#include "services/VehicleService.hpp"

#include <string>
#include <string_view>

struct DispatchResult
{
    bool success;
    std::string payload;
};

class ServiceBroker
{
public:
    [[nodiscard]] DispatchResult dispatch(std::string_view method) const;

private:
    VehicleService m_vehicleService;
};
