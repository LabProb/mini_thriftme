#pragma once

#include <string>

enum class MessageType
{
    Request,
    Response,
    Notification
};

struct RpcMessage
{
    MessageType type;

    std::string method;
    std::string payload;
};
