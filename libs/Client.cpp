#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib") 
#include <iostream>
#include <WinSock2.h>
#include <string>

#define DEFAULT_BUFLEN 512

class Sockets {
public:
    Sockets(int port, const char *ip);
    bool Init_socket();
    bool Send(std::string data);
    ~Sockets();

private:
    int port = 0;
    const char* ip;
    WSAData wsa_data;
    SOCKET this_socket;
    sockaddr_in Socket;
};

Sockets::Sockets(int port, const char* ip) {
    this->ip = ip;
    this->port = port;
}

bool Sockets::Init_socket() {
    if (WSAStartup(AF_INET, &wsa_data) != 0) return 1;
    this_socket = socket(AF_INET, SOCK_STREAM, NULL);
    if (this_socket == INVALID_SOCKET) return 1;

    Socket.sin_family = AF_INET;
    Socket.sin_addr.S_un.S_addr= inet_addr(ip);
    Socket.sin_port = htons(port);
    if (SOCKET_ERROR == connect(this_socket, (SOCKADDR*)&Socket, sizeof(Socket))) return 1;
    return 0;
}


bool Sockets::Send(std::string data){
    std::cout << "len = " << data.length() << std::endl;
    if (SOCKET_ERROR == send(this_socket, data.c_str(), data.length(), NULL)) return 1;
    return 0;

}

Sockets::~Sockets() {
    closesocket(this_socket);
    WSACleanup();
}





int main(){
    Sockets ip4(6001, "127.0.0.1");
    //char *buff;
    std::string data;

    if (ip4.Init_socket()) std::cout << "Init error" << std::endl;

    while (true) {
        getline(std::cin, data);
        std::cout << "For send: "<< data << std::endl;
        if (ip4.Send(data) == 0) std::cout << data << " -Sended" << std::endl;
        if (data == "exit") break;
    }
    system("pause");

    ip4.~Sockets();
}

