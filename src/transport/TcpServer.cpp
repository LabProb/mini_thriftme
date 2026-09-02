#include "TcpServer.hpp"
#include "common/RpcMessage.hpp"
#include "common/RpcResponse.hpp"
#include "SocketDescriptor.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <string>
#include <iostream>
#include <sys/socket.h>

namespace
{
constexpr std::size_t maxMessageSize = 64 * 1024;

bool sendAll(int socket, const std::string& data)
{
    std::size_t sent{};
    while (sent < data.size())
    {
        const auto bytes = send(socket, data.data() + sent, data.size() - sent, MSG_NOSIGNAL);
        if (bytes <= 0)
        {
            if (bytes < 0 && errno == EINTR)
            {
                continue;
            }

            return false;
        }

        sent += static_cast<std::size_t>(bytes);
    }

    return true;
}

RpcResponse invalidRequestResponse()
{
    return {0, false, "ERROR: Invalid request"};
}

bool receiveRequest(int socket, std::string& request)
{
    char buffer[4096];

    while (true)
    {
        const auto bytes = recv(socket, buffer, sizeof(buffer), 0);
        if (bytes == 0)
        {
            return !request.empty();
        }

        if (bytes < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            std::cerr << "Receiving request failed: " << std::strerror(errno) << '\n';
            return false;
        }

        if (request.size() + static_cast<std::size_t>(bytes) > maxMessageSize)
        {
            std::cerr << "Request exceeds the maximum supported size\n";
            return false;
        }

        request.append(buffer, static_cast<std::size_t>(bytes));
    }
}
} // namespace

void TcpServer::start(std::uint16_t port)
{
    SocketDescriptor serverSocket(socket(AF_INET, SOCK_STREAM, 0));

    if (!serverSocket)
    {
        std::cerr << "Socket creation failed: " << std::strerror(errno) << '\n';
        return;
    }

    const int reuseAddress = 1;
    if (setsockopt(serverSocket.get(), SOL_SOCKET, SO_REUSEADDR, &reuseAddress,
                   sizeof(reuseAddress)) < 0)
    {
        std::cerr << "Setting SO_REUSEADDR failed: " << std::strerror(errno) << '\n';
        return;
    }

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverSocket.get(),
             reinterpret_cast<sockaddr*>(&address),
             sizeof(address)) < 0)
    {
        std::cerr << "Bind failed: " << std::strerror(errno) << '\n';
        return;
    }

    if (listen(serverSocket.get(), SOMAXCONN) < 0)
    {
        std::cerr << "Listen failed: " << std::strerror(errno) << '\n';
        return;
    }

    std::cout
        << "TCP server listening on port "
        << port
        << '\n';

    while (true)
    {
        SocketDescriptor clientSocket(accept(serverSocket.get(), nullptr, nullptr));

        if (!clientSocket)
        {
            if (errno != EINTR)
            {
                std::cerr << "Accept failed: " << std::strerror(errno) << '\n';
            }

            continue;
        }

        std::string rawRequest;
        if (receiveRequest(clientSocket.get(), rawRequest))
        {
            const auto request = RpcMessage::deserialize(rawRequest);

            RpcResponse response = invalidRequestResponse();
            if (request)
            {
                std::cout << "Method: " << request->method << '\n';
                const auto result = m_broker.dispatch(request->method);
                response = {request->requestId, result.success, result.payload};
            }

            if (!sendAll(clientSocket.get(), response.serialize()))
            {
                std::cerr << "Sending response failed: " << std::strerror(errno) << '\n';
            }
        }
    }
}
