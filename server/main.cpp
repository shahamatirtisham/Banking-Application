#include "socket/server.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>\n";
        return 1;
    }

    int port = std::atoi(argv[1]);

    Server server(port);
    server.run();

    return 0;
}