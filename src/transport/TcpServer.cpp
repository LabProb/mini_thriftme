#include "TcpServer.hpp"
#include "broker/ServiceBroker.hpp"
#include "common/RpcMessage.hpp"
#include "common/RpcResponse.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

void TcpServer::start(int port)
{
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd < 0)
    {
        std::cerr << "Socket creation failed\n";
        return;
    }

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverFd,
             reinterpret_cast<sockaddr*>(&address),
             sizeof(address)) < 0)
    {
        std::cerr << "Bind failed\n";

        close(serverFd);

        return;
    }

    if (listen(serverFd, 5) < 0)
    {
        std::cerr << "Listen failed\n";

        close(serverFd);

        return;
    }

    std::cout
        << "TCP server listening on port "
        << port
        << '\n';

    int clientFd =
        accept(serverFd, nullptr, nullptr);

    if (clientFd < 0)
    {
        std::cerr << "Accept failed\n";

        close(serverFd);

        return;
    }

    char buffer[1024]{};

    ssize_t bytes =
        recv(clientFd, buffer, sizeof(buffer), 0);

    if (bytes > 0)
    {
        std::cout
            << "Received request: "
            << buffer
            << '\n';

        RpcMessage request =
            RpcMessage::deserialize(buffer);

        std::cout
            << "Method: "
            << request.method
            << '\n';

        ServiceBroker broker;

        std::string result =
            broker.dispatch(request.method);

        RpcResponse response;

        response.requestId =
            request.requestId;

        response.success = true;

        response.payload = result;

        std::string serialized =
            response.serialize();

        send(clientFd,
             serialized.c_str(),
             serialized.size(),
             0);
            }

    close(clientFd);
    close(serverFd);
}
