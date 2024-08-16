#include "client.h"

int main()
{
    Net::Client client(6666,"192.000.00.00"); // Need to enter your real server IpV4 and host
    client.clnconnect();
    return 0;
}