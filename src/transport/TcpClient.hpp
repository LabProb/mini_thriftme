#pragma once

#include <cstdint>
#include <string>

struct TcpClientResult
{
    std::string response;
    std::string error;

    [[nodiscard]] explicit operator bool() const noexcept
    {
        return error.empty();
    }
};

class TcpClient
{
public:
    [[nodiscard]] TcpClientResult sendRequest(const std::string& request,
                                              std::uint16_t port) const;
};
