#include "VehicleService.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>

#include <iostream>
#include <memory>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

int main()
{
    std::shared_ptr<TSocket>
        socket =
            std::make_shared<TSocket>(
                "127.0.0.1",
                9090);

    std::shared_ptr<TTransport>
        transport =
            std::make_shared<TBufferedTransport>(
                socket);

    std::shared_ptr<TProtocol>
        protocol =
            std::make_shared<TBinaryProtocol>(
                transport);

    vehicle::VehicleServiceClient client(
        protocol);

    try
    {
        transport->open();

        int speed =
            client.GetVehicleSpeed();

        double cpu =
            client.GetCpuUsage();

        double memory =
            client.GetMemoryUsage();

        std::cout
            << "Vehicle speed: "
            << speed
            << '\n';

        std::cout
            << "CPU usage: "
            << cpu
            << '\n';

        std::cout
            << "Memory usage: "
            << memory
            << '\n';

        transport->close();
    }
    catch (const TException& exception)
    {
        std::cerr
            << "Thrift exception: "
            << exception.what()
            << '\n';
    }

    return 0;
}
