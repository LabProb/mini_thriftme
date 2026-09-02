#include "broker/ServiceBroker.hpp"
#include "common/RpcMessage.hpp"
#include "common/RpcResponse.hpp"

#include <iostream>

namespace
{
bool check(bool condition, const char* expression)
{
    if (!condition)
    {
        std::cerr << "Check failed: " << expression << '\n';
    }

    return condition;
}
} // namespace

int main()
{
    const RpcMessage request{42, "GetVehicleSpeed", "data|with|separators"};
    const auto parsedRequest = RpcMessage::deserialize(request.serialize());
    if (!check(parsedRequest.has_value(), "parsedRequest.has_value()") ||
        !check(parsedRequest->requestId == 42, "parsedRequest->requestId == 42") ||
        !check(parsedRequest->method == "GetVehicleSpeed", "parsedRequest->method") ||
        !check(parsedRequest->payload == "data|with|separators", "parsedRequest->payload") ||
        !check(!RpcMessage::deserialize("not-a-request"), "invalid request is rejected") ||
        !check(!RpcMessage::deserialize("not-an-id|GetVehicleSpeed|"), "invalid id is rejected"))
    {
        return 1;
    }

    const RpcResponse response{42, false, "error|with|details"};
    const auto parsedResponse = RpcResponse::deserialize(response.serialize());
    if (!check(parsedResponse.has_value(), "parsedResponse.has_value()") ||
        !check(parsedResponse->requestId == 42, "parsedResponse->requestId == 42") ||
        !check(!parsedResponse->success, "!parsedResponse->success") ||
        !check(parsedResponse->payload == "error|with|details", "parsedResponse->payload") ||
        !check(!RpcResponse::deserialize("42|2|invalid-success"), "invalid success is rejected"))
    {
        return 1;
    }

    const ServiceBroker broker;
    const auto speed = broker.dispatch("GetVehicleSpeed");
    if (!check(speed.success, "speed.success") || !check(speed.payload == "72", "speed.payload"))
    {
        return 1;
    }

    const auto unknownMethod = broker.dispatch("UnknownMethod");
    if (!check(!unknownMethod.success, "!unknownMethod.success") ||
        !check(unknownMethod.payload == "ERROR: Unknown method", "unknownMethod.payload"))
    {
        return 1;
    }
}
