
#ifndef ERSSHW2_POSTSERVER_H
#define ERSSHW2_POSTSERVER_H

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

using namespace std;

class POSTServer : public Server {
private:
    ClientSocket clientSocket;
public:
    POSTServer(Request _r) : Server(_r), clientSocket(_r.getHostName(), strToVector("80")) {}

    void getResponse();

    void sendResponseToClient();

    void closeFd();
};

#endif //ERSSHW2_POSTSERVER_H
