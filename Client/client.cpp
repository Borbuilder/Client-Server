#include "client.h"

namespace Net
{
    Net::Client::Client(const unsigned short& _port, const std::string& _ipAdress)
        :
        wsa{0},
        clientSocket{INVALID_SOCKET},
        srv_ipAdress{_ipAdress},
        srv_port{_port},
        clnInfo{0},
        clientBuff(SIZE),
        messageLimit{0}
    {
    }

    Net::Client::~Client()
    {
        closesocket(clientSocket);
        WSACleanup();
    }

    void Net::Client::initialization()
    {
        std::cout << "WSA initialization : " << std::endl;
        if ( WSAStartup(MAKEWORD(2,2),&wsa) != 0  ) 
        {
            std::cout << "Couldn't initialize WSA" << std::endl;
        }
        else
        {
            std::cout << "Success with WSA initialization" << std::endl;
        }
    }

    void Net::Client::creation()
    {
        std::cout << "Socket initialization : " << std::endl;
        clientSocket = socket(AF_INET,SOCK_STREAM,0);
        if(clientSocket == INVALID_SOCKET)
        {
            std::cout << "Couldn't initialize socket" << std::endl;
        }
        else
        {
            std::cout << "Success with socket initialization" << std::endl;
        }
    }

    bool Net::Client::clnsend()
    {
        std::vector<char> serverBuff(SIZE);
        unsigned short sdpacket_size{};
        std::cout << "Your message :"<< std::endl;
        std::cin.getline(serverBuff.data(), serverBuff.size());

        sdpacket_size = send(clientSocket,serverBuff.data(),serverBuff.size(),0);
        if(sdpacket_size == SOCKET_ERROR)
        {
            std::cout << "Couldn't send message to server" << std::endl;
            return false;
        }
        else
        {
            return true;
        }
    } 

    void Net::Client::receive()
    {
        unsigned short rcpacket_size{};
        rcpacket_size = recv(clientSocket, clientBuff.data(), clientBuff.size(), 0 );
        if(rcpacket_size == SOCKET_ERROR)
        {
            std::cout << "Couldn't recieve message from client" << std::endl;
        }
        else
        {
            std::cout << "Message from server :" << std::endl;
            std::cout << " " << clientBuff.data() << std::endl;
        }
    } 

    void Net::Client::clnconnect()
    {
        initialization();
        creation();

        sockaddr_in srvInfo;
        int srvInfoSize = sizeof(srvInfo);
        memset(&clnInfo, 0 ,srvInfoSize);
        srvInfo.sin_family = AF_INET;
        srvInfo.sin_addr.s_addr = inet_addr(srv_ipAdress.c_str());
        srvInfo.sin_port = htons(srv_port);
       
        if( connect(clientSocket, reinterpret_cast<sockaddr*>(&srvInfo) , srvInfoSize) == INVALID_SOCKET )
        {
            std::cout << "Couldn't connect to a server" << std::endl;
        }
        else
        {
            std::cout << "Connected with " << srv_ipAdress <<" "<< srv_port << std::endl;
            
            std::vector<char> msgLimChar(sizeof(messageLimit));
            unsigned short limit_packet_size = recv(clientSocket,msgLimChar.data(),msgLimChar.size(),0);
            if(limit_packet_size == SOCKET_ERROR)
            {
                std::cout << "Couldn't send limit of messages to client" << std::endl;
            }
            else
            {
                std::memcpy(&messageLimit, msgLimChar.data(), sizeof(unsigned short));
                std::cout << "Limit of messages : " << messageLimit << std::endl;
                unsigned short msg_count{0};

                while(msg_count < messageLimit)
                {
                    clnsend();
                    receive();
                    ++msg_count;
                }
            }
            std::cout << "You were disconnected from the server due to exceeding the message limit" << std::endl;
        }
    }

}
