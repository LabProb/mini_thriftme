#include "common/RpcMessage.hpp"
#include "common/RpcResponse.hpp"
#include "transport/TcpClient.hpp"

#include <iostream>

int main()
{
    TcpClient client;

    RpcMessage request;

    request.requestId = 1;
    request.method = "GetVehicleSpeed";
    request.payload = "";

    const auto transportResult = client.sendRequest(request.serialize(), 8080);
    if (!transportResult)
    {
        std::cerr << "Request failed: " << transportResult.error << '\n';
        return 1;
    }

    const auto response = RpcResponse::deserialize(transportResult.response);
    if (!response)
    {
        std::cerr << "Server returned an invalid response\n";
        return 1;
    }

    std::cout
        << "Request ID: "
        << response->requestId
        << '\n';

    std::cout
        << "Success: "
        << response->success
        << '\n';

    std::cout
        << "Payload: "
        << response->payload
        << '\n';

    return 0;
}
