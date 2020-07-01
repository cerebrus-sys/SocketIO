/**
 * SocketIO
 * Author: Sanjan Geet Singh <sanjangeet2109s@gmail.com>
 */

#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <winsock.h>

//////////////////////////////////////////////////////////
// Windows Socket Library Initializer

struct SocketInitializer {

    SocketInitializer() {
        WSAData data;
        WSAStartup(MAKEWORD(2, 2), &data);
    }

    ~ SocketInitializer() {
        WSACleanup();
    }
};

SocketInitializer globalSocketInitializer;

//////////////////////////////////////////////////////////

class Socket {

private:
    int af;
    SOCKET s;

public:
    Socket();
    Socket(int af, int type);
    Socket(int af, int type, int protocol);
    ~Socket();
    bool is_valid();
    void close();
    bool connect(const char* ip, u_short port);
    void listen(int backlog);
    bool bind(u_short port);
    void operator=(SOCKET s);
    SOCKET accept();
    bool send(const char* msg, int size);
    const char* recv(int size);

};

Socket::Socket() {
    this->af = AF_INET;
    this->s = ::socket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(int af, int type) {
    this->af = af;
    this->s = ::socket(af, type, 0);
}

Socket::Socket(int af, int type, int protocol) {
    this->af = af;
    this->s = ::socket(af, type, protocol);
}

Socket::~Socket() {
    ::closesocket(this->s);
}

bool Socket::is_valid() {
    return this->s != INVALID_SOCKET;
}

void Socket::close() {
    ::closesocket(this->s);
}

bool Socket::connect(const char* ip, u_short port) {
    sockaddr_in addr;
    addr.sin_family = this->af;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    return ::connect(this->s, (sockaddr*)&addr, sizeof(addr)) == 0;
}

void Socket::listen(int backlog) {
    ::listen(this->s, backlog);
}

bool Socket::bind(u_short port) {
    sockaddr_in addr;
    addr.sin_family = this->af;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    return ::bind(this->s, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR;
}

void Socket::operator=(SOCKET s) {
    this->s = s;
}

SOCKET Socket::accept() {
    int s = sizeof(sockaddr_in);
    SOCKET n;
    sockaddr_in addr;

    n = ::accept(this->s, (sockaddr*)&addr, &s);
    return n;
}

bool Socket::send(const char* msg, int size) {
    return ::send(this->s, msg, size, 0) > 0;
}

const char* Socket::recv(int size) {
    char* msg = new char(size);
    int s = ::recv(this->s, msg, size, 0);
    msg[s] = '\0';
    return msg;
}

#endif // __SOCKET_HPP__
