#include "HttpCachingProxy.h"

void HttpCachingProxy::runProxy() {
    serverSocket.setUp();
    while (true) {
        int clientFd = serverSocket.toAccept();
        thread th(&HttpCachingProxy::handleNewRequest, this, clientFd);
        th.detach();
    }
}

void HttpCachingProxy::handleNewRequest(int clientFd) {
    //receive buffer
    vector<char> buffer;
    try {
        buffer = ServerSocket::toReceive(clientFd);
    }catch(MyException &exp){
        std::cout << exp.what();
    }
    if (buffer.empty()) {
        return;
    }

    m.lock();
    string newId = this->uniqueId->updateID();
    m.unlock();
    try {
        Request newRequest(clientFd, serverSocket.getFd(), newId, buffer);
        if (newRequest.getType() == GET) {
            GETServer gs(newRequest, myCache);
            int status = gs.checkCache();
            if(status == NOTINCACHE || status == UNKNOWN) {
                gs.getResponse();
                gs.sendResponseToClient();
                gs.updateCache();
            }
            gs.closeFd();
        } else if (newRequest.getType() == CONNECT) {
            CONNECTServer cs(newRequest);
            cs.connectToTargetServer();
            cs.sendResponseBackToClient();
            cs.sendBytesBackAndForth();
            cs.closeFd();
        } else if (newRequest.getType() == POST) {
            POSTServer ps(newRequest);
            ps.getResponse();
            ps.sendResponseToClient();
            ps.closeFd();
        }
    }catch (MyException &exp){
        std::cout << exp.what();
    }
}

int main() {
    HttpCachingProxy httpCachingProxy(make_shared<UniqueID>(6), make_shared<Cache>(50));
    httpCachingProxy.runProxy();
    PROXYLOG.close();
    return EXIT_SUCCESS;
}
