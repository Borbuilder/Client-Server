#include<iostream>
#include<vector>
#include<cstring>
#include<string>
#include<winsock2.h>
#include<Windows.h>


#pragma comment(lib, "Ws2_32.lib");
//#pragma warning(disable: 4996)

#define SIZE 1024

namespace Net
{
    class Server
    {
        private:
            WSADATA wsa;
            SOCKET serverSocket;
            std::string ipAdress;
            unsigned short port;
            sockaddr_in srvInfo;
            unsigned short messageLimit{};
            std::vector<char> serverBuff;

        private:
            void initialization();
            void creation();
            void binding();
            void receive(const char* _clnAdr, const u_short& _clnPort, const SOCKET& clnSock);
            void srvsend(const SOCKET& clnSock);

        public:
            Server(const unsigned short& _port, const std::string& _ipAdress);
            ~Server();
            void start();
    };
}