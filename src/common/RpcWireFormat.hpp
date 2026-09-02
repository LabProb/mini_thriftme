#pragma once

#include <charconv>
#include <optional>
#include <string>
#include <string_view>

namespace rpc::detail
{
inline std::optional<std::pair<std::string_view, std::string_view>> splitField(
    std::string_view input)
{
    const auto separator = input.find('|');
    if (separator == std::string_view::npos)
    {
        return std::nullopt;
    }

    return std::pair{input.substr(0, separator), input.substr(separator + 1)};
}

inline std::optional<int> parseInt(std::string_view text)
{
    int value{};
    const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);

    if (error != std::errc{} || end != text.data() + text.size())
    {
        return std::nullopt;
    }

    return value;
}
} // namespace rpc::detail
