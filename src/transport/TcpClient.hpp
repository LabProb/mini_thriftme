#pragma once

#include <string>

class TcpClient
{
public:
    std::string sendRequest(const std::string& request,
                            int port);
};
