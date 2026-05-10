#include "TcpClient.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

std::string TcpClient::sendRequest(
    const std::string& request,
    int port)
{
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd < 0)
    {
        return "Socket creation failed";
    }

    sockaddr_in serverAddr{};

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    inet_pton(AF_INET,
              "127.0.0.1",
              &serverAddr.sin_addr);

    if (connect(sockFd,
                reinterpret_cast<sockaddr*>(&serverAddr),
                sizeof(serverAddr)) < 0)
    {
        close(sockFd);

        return "Connection failed";
    }

    send(sockFd,
         request.c_str(),
         request.size(),
         0);

    char buffer[1024]{};

    ssize_t bytes =
        recv(sockFd, buffer, sizeof(buffer), 0);

    close(sockFd);

    if (bytes > 0)
    {
        return std::string(buffer, bytes);
    }

    return "No response";
}
