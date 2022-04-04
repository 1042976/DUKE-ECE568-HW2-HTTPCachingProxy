
#ifndef ERSSHW2_HTTPCACHINGPROXY_H
#define ERSSHW2_HTTPCACHINGPROXY_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <utility>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <memory>
#include "MyException.h"
#include "MyData.h"
#include "UniqueID.h"
#include "ServerSocket.h"
#include "Request.h"
#include "GETServer.h"
#include "POSTServer.h"
#include "CONNECTServer.h"
#include "Response.h"
#include "Cache.h"
using namespace std;

class HttpCachingProxy {
private:

public:
    shared_ptr <UniqueID> uniqueId;
    shared_ptr <Cache> myCache;
    ServerSocket serverSocket;

    HttpCachingProxy(shared_ptr <UniqueID> _uniqueId, shared_ptr<Cache> _myCache) : uniqueId(_uniqueId),myCache(_myCache),
                                                        serverSocket(strToVector(MYPORT), BACKLOG) {};

    //-----------------------------
    void runProxy();

    void handleNewRequest(int clientFd);

};

#endif //ERSSHW2_HTTPCACHINGPROXY_H