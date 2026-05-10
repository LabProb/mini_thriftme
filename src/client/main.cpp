#include "common/RpcMessage.hpp"
#include "transport/TcpClient.hpp"

#include <iostream>

int main()
{
    TcpClient client;

    RpcMessage request;

    request.requestId = 1;
    request.method = "GetVehicleSpeed";
    request.payload = "";

    std::string response =
        client.sendRequest(
            request.serialize(),
            8080);

    std::cout
        << "Server response: "
        << response
        << '\n';

    return 0;
}
