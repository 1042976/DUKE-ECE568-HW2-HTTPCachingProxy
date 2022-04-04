#include "POSTServer.h"


void POSTServer::getResponse() {
    logRequestInfo();
    clientSocket.setSocket();
    clientSocket.toConnect();
    clientSocket.toSend(r.getAllContent());
    auto len_buf= clientSocket.toReceive();
    rs = Response(r.getUniqueId(), len_buf.first, len_buf.second);
    while(rs.lessThanContentLength()){
        auto next_len_buf= clientSocket.toReceive();
        if(next_len_buf.first == 0){
            break;
        }
        rs.addContent(next_len_buf.first, next_len_buf.second);
    }
    if(rs.isChunked()){
        while(true){
            auto next_len_buf= clientSocket.toReceive();
            if(next_len_buf.first == 0){
                break;
            }
            rs.addContent(next_len_buf.first, next_len_buf.second);
        }
    }
    logReceivedRespond();
}

void POSTServer::sendResponseToClient() {
    int status = 0;
    vector<char> responseInfo = rs.getAllContent();
    status = send(r.getClientFd(), responseInfo.data(), rs.getBufferLen(), 0);
    if(status == -1){
        logError(r.getUniqueId(), "Fail to send response to the client!");
        throw MyException();
    }
    logResponding(vectorToStr(rs.getFirstLine()));
}

void POSTServer::closeFd() {
    close(r.getClientFd());
    close(clientSocket.getFd());
}