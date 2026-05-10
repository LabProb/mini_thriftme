#include "transport/TcpClient.hpp"

#include <iostream>

int main()
{
    TcpClient client;

    std::string response =
        client.sendRequest(
            "GetVehicleSpeed",
            8080);

    std::cout
        << "Server response: "
        << response
        << '\n';

    return 0;
}
