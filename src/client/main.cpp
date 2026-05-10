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

    std::string rawResponse =
        client.sendRequest(
            request.serialize(),
            8080);

    RpcResponse response =
        RpcResponse::deserialize(
            rawResponse);

    std::cout
        << "Request ID: "
        << response.requestId
        << '\n';

    std::cout
        << "Success: "
        << response.success
        << '\n';

    std::cout
        << "Payload: "
        << response.payload
        << '\n';

    return 0;
}
