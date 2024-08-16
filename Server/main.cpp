#include "server.h"

int main()
{
    Net::Server server(6666,"192.000.00.00"); // Need to enter your real server IpV4 and host
    server.start();
    return 0;
}