#include "server.h"

namespace Net
{
    Net::Server::Server(const unsigned short& _port, const std::string& _ipAdress)
        :
        wsa{0},
        serverSocket{INVALID_SOCKET},
        ipAdress{_ipAdress},
        port{_port},
        srvInfo{0},
        messageLimit{10},
        serverBuff(SIZE)
    {
    }

    Net::Server::~Server()
    {
        closesocket(serverSocket);
        WSACleanup();
    }

    void Net::Server::initialization()
    {                           
        std::cout << "WSA initialization : " << std::endl;
        if ( (WSAStartup(MAKEWORD(2,2),&wsa)) != 0) 
        {
            std::cout << "Couldn't initialize WSA" << std::endl;
        }
        else
        {
            std::cout << "Success with WSA initialization" << std::endl;
        }
    }

    void Net::Server::creation()
    {
        std::cout << "Socket initialization : " << std::endl;
        serverSocket = socket(AF_INET,SOCK_STREAM,0);
        if(serverSocket == INVALID_SOCKET)
        {
            std::cout << "Couldn't initialize socket" << std::endl;
        }
        else
        {
            std::cout << "Success with socket initialization" << std::endl;
        }
    }

    void Net::Server::binding()
    {
        srvInfo.sin_family = AF_INET;
        srvInfo.sin_port = htons(port);
        srvInfo.sin_addr.s_addr = inet_addr(ipAdress.c_str());
        
        std::cout << "Socket binding : " << std::endl;
        if( bind(serverSocket, reinterpret_cast<sockaddr*>(&srvInfo), sizeof(srvInfo)) == SOCKET_ERROR )
        {
            std::cout << "Couldn't bind socket" << std::endl;
        }
        else
        {
            std::cout << "Success with socket binding" << std::endl;
        }

        std::cout << "Server started on " << inet_ntoa(srvInfo.sin_addr) << " " << ntohs(srvInfo.sin_port) << std::endl; 
    }

    void Net::Server::receive(const char* _clnAdr, const u_short& _clnPort, const SOCKET& clnSock)
    {
        unsigned short rcpacket_size{};
        rcpacket_size = recv(clnSock, serverBuff.data(), serverBuff.size(), 0 );
        if(rcpacket_size == SOCKET_ERROR)
        {
            std::cout << "Couldn't recieve message from client" << std::endl;
        }
        else
        {
            std::cout << "Message from " << _clnAdr << " " << _clnPort << " :" << std::endl;
            std::cout << " " << serverBuff.data() << std::endl;
        }
    }

    void Net::Server::srvsend(const SOCKET& clnSock)
    {
        std::vector<char> clientBuff(SIZE);
        unsigned short sdpacket_size{};
        std::cout << "Server answer :"<< std::endl;
        std::cin.getline(clientBuff.data(), clientBuff.size());

        sdpacket_size = send(clnSock,clientBuff.data(),clientBuff.size(),0);
        if(sdpacket_size == SOCKET_ERROR)
        {
            std::cout << "Couldn't send message to client" << std::endl;
        }
    }   

    void Net::Server::start()
    {
        initialization();
        creation();
        binding();

        if( listen(serverSocket,SOMAXCONN) == SOCKET_ERROR)
        {
            std::cout << "Can't listen" << std::endl;
            std::cout <<  WSAGetLastError() << std::endl;
        }
        else
        {
            std::cout << "Listening..." << std::endl;
            sockaddr_in clnInfo;
            int clnInfoSize = sizeof(clnInfo);
            memset(&clnInfo, 0 ,clnInfoSize);
            SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clnInfo) , &clnInfoSize);
            const char* clnAdr = inet_ntoa(clnInfo.sin_addr);
            u_short clnPort = ntohs(clnInfo.sin_port);
            unsigned short msg_count{0};

            if(clientSocket == INVALID_SOCKET)
            {
                std::cout << "Client is detected, but can't connect to a client" << std::endl;
            }
            else
            {
                std::cout << "Connected with " << clnAdr <<" "<< clnPort << std::endl;
    
                std::vector<char> msgLimChar(sizeof(messageLimit));
                std::memcpy(msgLimChar.data(), &messageLimit, sizeof(messageLimit));
                unsigned short limit_packet_size = send(clientSocket,msgLimChar.data(),msgLimChar.size(),0);

                if(limit_packet_size == SOCKET_ERROR)
                {
                    std::cout << "Couldn't send limit of messages to client" << std::endl;
                }
                else
                {
                    while(msg_count < messageLimit)
                    {
                        receive(clnAdr,clnPort,clientSocket);
                        srvsend(clientSocket);
                        ++msg_count;
                    }
                }
                closesocket(clientSocket);
            }
            
        }

        std::cout << "Server finished" <<std::endl;

    }
}



