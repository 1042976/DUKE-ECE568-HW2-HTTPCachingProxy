#ifndef ERSSHW2_CONNECTSERVER_H
#define ERSSHW2_CONNECTSERVER_H

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <algorithm>
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

class CONNECTServer : public Server {
private:
    ClientSocket clientSocket;
public:
    CONNECTServer(Request _r) : Server(_r), clientSocket(_r.getHostName(), strToVector("443")) {}

    void connectToTargetServer();

    void sendResponseBackToClient();

    void sendBytesBackAndForth();

    void closeFd();
};

#endif //ERSSHW2_CONNECTIONSERVER_H