#include "CONNECTServer.h"

void CONNECTServer::connectToTargetServer() {
    logRequestInfo();
    clientSocket.setSocket();
    clientSocket.toConnect();
}

void CONNECTServer::sendResponseBackToClient() {
    int status = 0;
    vector<char> response = strToVector("HTTP/1.1 200 OK\r\n\r\n");
    logResponding("HTTP/1.1 200 OK");
    status = send(r.getClientFd(), response.data(), response.size(), 0);
    if (status == -1) {
        logError(r.getUniqueId(), "Fail to send 200 OK back to client!");
        throw MyException();
    }
}

void CONNECTServer::sendBytesBackAndForth() {
    int status = 0;
    int clientFd = r.getClientFd();
    int targetFd = clientSocket.getFd();
    while (true) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(clientFd, &set);
        FD_SET(targetFd, &set);
        int maxFd = max(targetFd, clientFd);
        status = select(maxFd + 1, &set, nullptr, nullptr, nullptr);
        if (status == -1) {
            throw MyException("Error: select function fails!");
        }
        int startFd = -1, endFd = -1;
        if (FD_ISSET(clientFd, &set) != 0) {
            startFd = clientFd;
            endFd = targetFd;
        } else if (FD_ISSET(targetFd, &set) != 0) {
            startFd = targetFd;
            endFd = clientFd;
        } else {
            break;
        }
        vector<char> buffer(100000, '\0');
        int bufferLen = recv(startFd, buffer.data(), buffer.size(), 0);
        standardizeVector(buffer);
        if (bufferLen == -1) {
            throw MyException("Error: fail to receive data!");
        } else if (bufferLen == 0) {
            break;
        }
        status = send(endFd, buffer.data(), bufferLen, 0);
        if (status == -1) {
            throw MyException("Error: fail to send data!");
        }
    }
    logTunnelClosed();
}

void CONNECTServer::closeFd() {
    close(r.getClientFd());
    close(clientSocket.getFd());
}