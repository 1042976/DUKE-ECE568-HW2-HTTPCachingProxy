#include "ServerSocket.h"

ServerSocket::ServerSocket(vector<char> _port, int _backlog) : fd(-1), backlog(_backlog), port(_port) {}


int ServerSocket::getFd() {
    return fd;
}

int ServerSocket::getBacklog() {
    return backlog;
}

void ServerSocket::setSocket() {
    memset(&hints, 0, sizeof(hints));
    string error_message = "Error: Unable to create a server socket!";
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    setZeroInTheEnd(port);
    if (getaddrinfo(hostname.data(), port.data(), &hints, &servinfo) != 0) {
        throw MyException("Error: Unable to create a server socket!");
    }
    fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (fd == -1) {
        throw MyException("Error: Unable to create a server socket!");
    }
}

void ServerSocket::setUp() {
    setSocket();
    toSetSockOpt();
    toBind();
    toListen();
}

void ServerSocket::toSetSockOpt() {
    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0) {
        throw MyException("Error: Fail to set socket option!");
    }
}

void ServerSocket::toBind() {
    if (bind(fd, servinfo->ai_addr, servinfo->ai_addrlen) != 0) {
        throw MyException("Error: Fail to bind");
    }
}

void ServerSocket::toListen() {
    if (listen(fd, backlog) != 0) {
        throw MyException("Error: Fail to listen");
    }
}

int ServerSocket::toAccept() {
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    if ((new_socket = accept(fd, (struct sockaddr *) &address,
                             (socklen_t * ) & addrlen)) < 0) {
        throw MyException("Error: Fail to accept new socket");
    }
    return new_socket;
}

vector<char> ServerSocket::toReceive(int new_fd) {
    vector<char> buffer(100000);
    int bufferlen = recv(new_fd, buffer.data(), buffer.size(), 0);
    if (bufferlen < 0) {
        throw MyException("Error: fail to receive header");
    } else if (bufferlen == 0) {
        return vector < char > {};
    }
    standardizeVector(buffer);
    return buffer;
}