#include "VehicleService.h"

#include "services/VehicleService.hpp"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TServerSocket.h>

#include <iostream>
#include <memory>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::server;
using namespace apache::thrift::transport;

class VehicleServiceHandler
    : public vehicle::VehicleServiceIf
{
public:
    int32_t GetVehicleSpeed() override
    {
        std::cout
            << "GetVehicleSpeed called\n";

        return 72;
    }

    double GetCpuUsage() override
    {
        return 15.5;
    }

    double GetMemoryUsage() override
    {
        return 42.3;
    }
};

int main()
{
    std::shared_ptr<VehicleServiceHandler>
        handler =
            std::make_shared<
                VehicleServiceHandler>();

    std::shared_ptr<TProcessor>
        processor =
            std::make_shared<
                vehicle::VehicleServiceProcessor>(
                    handler);

    std::shared_ptr<TServerTransport>
        serverTransport =
            std::make_shared<TServerSocket>(
                9090);

    std::shared_ptr<TTransportFactory>
        transportFactory =
            std::make_shared<
                TBufferedTransportFactory>();

    std::shared_ptr<TProtocolFactory>
        protocolFactory =
            std::make_shared<
                TBinaryProtocolFactory>();

    TSimpleServer server(
        processor,
        serverTransport,
        transportFactory,
        protocolFactory);

    std::cout
        << "Thrift server started on port 9090\n";

    server.serve();

    return 0;
}
