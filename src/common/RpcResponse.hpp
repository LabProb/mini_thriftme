#pragma once

#include <sstream>
#include <string>

struct RpcResponse
{
    int requestId {};

    bool success {};

    std::string payload;

    std::string serialize() const
    {
        std::ostringstream stream;

        stream
            << requestId
            << '|'
            << success
            << '|'
            << payload;

        return stream.str();
    }

    static RpcResponse deserialize(
        const std::string& data)
    {
        RpcResponse response;

        std::stringstream stream(data);

        std::string requestIdStr;
        std::string successStr;

        std::getline(
            stream,
            requestIdStr,
            '|');

        response.requestId =
            std::stoi(requestIdStr);

        std::getline(
            stream,
            successStr,
            '|');

        response.success =
            std::stoi(successStr);

        std::getline(
            stream,
            response.payload);

        return response;
    }
};