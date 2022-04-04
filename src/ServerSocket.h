#ifndef ERSSHW2_SERVERSOCKET_H
#define ERSSHW2_SERVERSOCKET_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <memory>
#include <utility>
#include <vector>
#include "string"
#include "cstring"
#include "MyException.h"
#include "MyData.h"

using namespace std;

class ServerSocket {
private:
    int fd, backlog;
    struct addrinfo hints, *servinfo;
    std::vector<char> hostname, port;
public:
    ServerSocket(vector<char> _port, int _backlog);

    int getFd();

    int getBacklog();

    void setSocket();

    void toSetSockOpt();

    void toBind();

    void toListen();

    int toAccept();

    void setUp();

    static vector<char> toReceive(int new_fd);

};

#endif //ERSSHW2_SERVERSOCKET_H