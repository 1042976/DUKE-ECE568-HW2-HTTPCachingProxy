#ifndef ERSSHW2_CLIENTSOCKET_H
#define ERSSHW2_CLIENTSOCKET_H

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
#include "Response.h"

class ClientSocket {
private:
    int fd;
    struct addrinfo hints, *servinfo;
    std::vector<char> hostname, port;

public:
    ClientSocket(vector<char> _hostname, vector<char> _port);

    int getFd();

    vector<char> getHostName();

    vector<char> getPort();

    void setSocket();

    void toConnect();

    void toSend(vector<char> allContent);

    pair<int, vector<char>> toReceive();
};

#endif //ERSSHW2_CLIENTSOCKET_H
