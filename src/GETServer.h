#ifndef ERSSHW2_GETSERVER_H
#define ERSSHW2_GETSERVER_H

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <unistd.h>

#include "MyException.h"
#include "MyData.h"
#include "Request.h"
#include "Server.h"
#include "ClientSocket.h"
#include "ServerSocket.h"
#include "Cache.h"
using namespace std;

class GETServer : public Server {
private:
    ClientSocket clientSocket;
    shared_ptr<Cache> myCache;
public:
    GETServer(Request _r, shared_ptr<Cache> _myCache) : Server(_r), clientSocket(_r.getHostName(), strToVector("80")), myCache(_myCache) {}

    int checkCache();

    void getResponse();

    void sendResponseToClient();

    void updateCache();

    void closeFd();

};

#endif //ERSSHW2_GETSERVER_H