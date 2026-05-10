#pragma once

#include <sstream>
#include <string>

struct RpcMessage
{
    int requestId {};

    std::string method;

    std::string payload;

    std::string serialize() const
    {
        std::ostringstream stream;

        stream
            << requestId
            << '|'
            << method
            << '|'
            << payload;

        return stream.str();
    }

    static RpcMessage deserialize(
        const std::string& data)
    {
        RpcMessage message;

        std::stringstream stream(data);

        std::string requestIdStr;

        std::getline(
            stream,
            requestIdStr,
            '|');

        message.requestId =
            std::stoi(requestIdStr);

        std::getline(
            stream,
            message.method,
            '|');

        std::getline(
            stream,
            message.payload);

        return message;
    }
};
