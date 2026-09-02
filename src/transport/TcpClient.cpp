#include "TcpClient.hpp"
#include "SocketDescriptor.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
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

TcpClientResult receiveResponse(int socket)
{
    std::string response;
    char buffer[4096];

    while (true)
    {
        const auto bytes = recv(socket, buffer, sizeof(buffer), 0);
        if (bytes == 0)
        {
            return response.empty() ? TcpClientResult{{}, "Connection closed without a response"}
                                    : TcpClientResult{std::move(response), {}};
        }

        if (bytes < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            return {{}, "Receiving response failed: " + std::string(std::strerror(errno))};
        }

        if (response.size() + static_cast<std::size_t>(bytes) > maxMessageSize)
        {
            return {{}, "Response exceeds the maximum supported size"};
        }

        response.append(buffer, static_cast<std::size_t>(bytes));
    }
}
} // namespace

TcpClientResult TcpClient::sendRequest(
    const std::string& request,
    std::uint16_t port) const
{
    SocketDescriptor socketDescriptor(socket(AF_INET, SOCK_STREAM, 0));

    if (!socketDescriptor)
    {
        return {{}, "Socket creation failed: " + std::string(std::strerror(errno))};
    }

    sockaddr_in serverAddr{};

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) != 1)
    {
        return {{}, "Invalid loopback address"};
    }

    if (connect(socketDescriptor.get(),
                reinterpret_cast<sockaddr*>(&serverAddr),
                sizeof(serverAddr)) < 0)
    {
        return {{}, "Connection failed: " + std::string(std::strerror(errno))};
    }

    if (!sendAll(socketDescriptor.get(), request))
    {
        return {{}, "Sending request failed: " + std::string(std::strerror(errno))};
    }

    if (shutdown(socketDescriptor.get(), SHUT_WR) < 0)
    {
        return {{}, "Finishing request failed: " + std::string(std::strerror(errno))};
    }

    return receiveResponse(socketDescriptor.get());
}
