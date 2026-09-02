#pragma once

#include "RpcWireFormat.hpp"

#include <optional>
#include <string>
#include <string_view>

struct RpcResponse
{
    int requestId {};

    bool success {};

    std::string payload;

    [[nodiscard]] std::string serialize() const
    {
        return std::to_string(requestId) + '|' + (success ? "1" : "0") + '|' + payload;
    }

    [[nodiscard]] static std::optional<RpcResponse> deserialize(std::string_view data)
    {
        const auto idAndRemaining = rpc::detail::splitField(data);
        if (!idAndRemaining)
        {
            return std::nullopt;
        }

        const auto successAndPayload = rpc::detail::splitField(idAndRemaining->second);
        const auto requestId = rpc::detail::parseInt(idAndRemaining->first);
        if (!successAndPayload || !requestId ||
            (successAndPayload->first != "0" && successAndPayload->first != "1"))
        {
            return std::nullopt;
        }

        return RpcResponse{*requestId, successAndPayload->first == "1",
                           std::string(successAndPayload->second)};
    }
};
