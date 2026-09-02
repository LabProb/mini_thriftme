#pragma once

#include "RpcWireFormat.hpp"

#include <optional>
#include <string>
#include <string_view>

struct RpcMessage
{
    int requestId {};

    std::string method;

    std::string payload;

    [[nodiscard]] std::string serialize() const
    {
        return std::to_string(requestId) + '|' + method + '|' + payload;
    }

    [[nodiscard]] static std::optional<RpcMessage> deserialize(std::string_view data)
    {
        const auto idAndRemaining = rpc::detail::splitField(data);
        if (!idAndRemaining)
        {
            return std::nullopt;
        }

        const auto methodAndPayload = rpc::detail::splitField(idAndRemaining->second);
        const auto requestId = rpc::detail::parseInt(idAndRemaining->first);
        if (!methodAndPayload || !requestId || methodAndPayload->first.empty())
        {
            return std::nullopt;
        }

        return RpcMessage{*requestId, std::string(methodAndPayload->first),
                          std::string(methodAndPayload->second)};
    }
};
