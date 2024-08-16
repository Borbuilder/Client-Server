#include<iostream>
#include<vector>
#include<string>
#include<cstring>
#include<winsock2.h>
#include<Windows.h>


#pragma comment(lib, "Ws2_32.lib");
//#pragma warning(disable: 4996)

#define SIZE 1024

namespace Net
{
    class Client
    {
        private:
            WSADATA wsa;
            SOCKET clientSocket;
            std::string srv_ipAdress;
            unsigned short srv_port;
            sockaddr_in clnInfo;
            std::vector<char> clientBuff;
            unsigned short messageLimit;

        private:
            void initialization();
            void creation();
            void receive();
            bool clnsend();

        public:
            Client(const unsigned short& _port, const std::string& _ipAdress);
            ~Client();
            void clnconnect();
    };
}