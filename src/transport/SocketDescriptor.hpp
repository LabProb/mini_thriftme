#pragma once

#include <unistd.h>

class SocketDescriptor
{
public:
    explicit SocketDescriptor(int descriptor = -1) noexcept
        : m_descriptor(descriptor)
    {
    }

    ~SocketDescriptor()
    {
        reset();
    }

    SocketDescriptor(const SocketDescriptor&) = delete;
    SocketDescriptor& operator=(const SocketDescriptor&) = delete;

    SocketDescriptor(SocketDescriptor&& other) noexcept
        : m_descriptor(other.release())
    {
    }

    SocketDescriptor& operator=(SocketDescriptor&& other) noexcept
    {
        if (this != &other)
        {
            reset(other.release());
        }

        return *this;
    }

    [[nodiscard]] int get() const noexcept
    {
        return m_descriptor;
    }

    [[nodiscard]] explicit operator bool() const noexcept
    {
        return m_descriptor >= 0;
    }

    [[nodiscard]] int release() noexcept
    {
        const int descriptor = m_descriptor;
        m_descriptor = -1;
        return descriptor;
    }

    void reset(int descriptor = -1) noexcept
    {
        if (*this)
        {
            close(m_descriptor);
        }

        m_descriptor = descriptor;
    }

private:
    int m_descriptor;
};
