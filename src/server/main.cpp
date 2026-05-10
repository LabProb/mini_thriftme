#include "transport/TcpServer.hpp"

int main()
{
    TcpServer server;

    server.start(8080);

    return 0;
}
