#pragma once

#include "broker/ServiceBroker.hpp"

#include <cstdint>

class TcpServer
{
public:
    void start(std::uint16_t port);

private:
    ServiceBroker m_broker;
};
