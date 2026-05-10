#include "TcpServer.hpp"
#include "broker/ServiceBroker.hpp"

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

        ServiceBroker broker;

        std::string response =
            broker.dispatch(buffer);

        send(clientFd,
             response.c_str(),
             response.size(),
             0);
    }

    close(clientFd);
    close(serverFd);
}
