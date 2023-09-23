#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib") 
#include <iostream>
#include <WinSock2.h>
#include <time.h>

#define DEFAULT_BUFLEN 512

bool recv_flag = false;

class Sockets {
public:
    Sockets(int port, const char* ip);
    bool Init_client_socket();
    bool Init_server_socket();
    template <typename T>
    bool Send(T data);
    char* Recieve();
    ~Sockets();

private:
    int port = 0;
    const char* ip;
    WSAData wsa_data;
    SOCKET this_socket;
    sockaddr_in Socket;
    SOCKET client_socket;
};

Sockets::Sockets(int port, const char* ip) {
    this->ip = ip;
    this->port = port;
}

bool Sockets::Init_client_socket() {
    if (WSAStartup(AF_INET, &wsa_data) != 0) return 1;
    this_socket = socket(AF_INET, SOCK_STREAM, NULL);
    if (this_socket == INVALID_SOCKET) return 1;

    Socket.sin_family = AF_INET;
    Socket.sin_addr.S_un.S_addr = inet_addr(ip);
    Socket.sin_port = htons(port);
    if (SOCKET_ERROR == connect(this_socket, (SOCKADDR*)&Socket, sizeof(Socket))) return 1;
    return 0;
}

bool Sockets::Init_server_socket() {
    if (WSAStartup(AF_INET, &wsa_data) != 0) return 1;
    this_socket = socket(AF_INET, SOCK_STREAM, NULL);
    if (this_socket == INVALID_SOCKET) return 1;

    Socket.sin_family = AF_INET;
    Socket.sin_addr.S_un.S_addr = inet_addr(ip);
    Socket.sin_port = htons(port);

    bind(this_socket, (SOCKADDR*)&Socket, sizeof(Socket));
    listen(this_socket, 0x100);
    int socket_len = sizeof(Socket);
    client_socket = accept(this_socket, (SOCKADDR*)&Socket, &socket_len);
    return 0;
}

template<typename T>
bool Sockets::Send(T data) {
    const char* dat = "R";
    if (SOCKET_ERROR == send(this_socket, dat, sizeof(dat), NULL)) return 1;
    return 0;

}

char* Sockets::Recieve() {
    char data[DEFAULT_BUFLEN];
    int lenght = 0;
    if (lenght = recv(client_socket, data, 128, NULL) > 0)  recv_flag = true;
    std::cout << "len =" << lenght << std::endl;

    return data;
}

Sockets::~Sockets() {
    closesocket(this_socket);
    WSACleanup();
}


int main(){
    Sockets ip4(6001, "127.0.0.1");
    char *buff;
    ip4.Init_server_socket();

    struct tm* ptr;
    time_t Time;

    while (true) {
    Time = time(NULL);
        ptr = localtime(&Time);
        buff = ip4.Recieve();

        if (buff == "exit") break;
        if (recv_flag == true) std::cout << '[' << ptr->tm_year <<'.' << ptr->tm_mon << '.' << ptr->tm_mday 
                                         << '-' << ptr->tm_hour << ':' << ptr->tm_min << ':' << ptr->tm_sec << "]- " 
                                         << buff << std::endl;
        recv_flag = false;

    }
    ip4.~Sockets();

}
