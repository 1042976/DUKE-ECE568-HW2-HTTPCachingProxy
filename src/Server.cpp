#include "Server.h"

void Server::setResponse(const Response &otherRs) {
    this->rs = otherRs;
}

void Server::logRequestInfo() {
    cout << r.getUniqueIDWithColon() + "Requesting \"" <<vectorToStr(r.getFirstLine()) << "\" from " << vectorToStr(r.getHostName()) << endl;
    PROXYLOG << r.getUniqueIDWithColon() + "Requesting \"" <<vectorToStr(r.getFirstLine()) << "\" from " << vectorToStr(r.getHostName()) << endl;
}

void Server::logResponding(string responding) {
    cout << r.getUniqueIDWithColon() << "Responding " << addQuotationMarks(responding) << endl;
    PROXYLOG <<  r.getUniqueIDWithColon() << "Responding " << addQuotationMarks(responding) << endl;
}

void Server::logReceivedRespond() {
    cout << r.getUniqueIDWithColon() << "Received " <<addQuotationMarks(vectorToStr(rs.getFirstLine())) << " from " << vectorToStr(r.getHostName())<<endl;
    PROXYLOG << r.getUniqueIDWithColon() << "Received " << addQuotationMarks(vectorToStr(rs.getFirstLine())) << " from " << vectorToStr(r.getHostName())<< endl;
}

void Server::logTunnelClosed() {
    cout << r.getUniqueIDWithColon() << "Tunnel closed" << endl;
}