#ifndef ERSSHW2_SERVER_H
#define ERSSHW2_SERVER_H

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
#include "Response.h"
class Server {
protected:
    Request r;
    Response rs;
public:
    Server(Request _r) : r(_r) {
        cout << _r.getFirstLineIPTime() << endl;
        PROXYLOG << _r.getFirstLineIPTime() << endl;
    }
    void setResponse(const Response& otherRs);
    void logRequestInfo();
    void logReceivedRespond();
    void logResponding(string responding);
    void logTunnelClosed();
};

#endif //ERSSHW2_SERVER_H
