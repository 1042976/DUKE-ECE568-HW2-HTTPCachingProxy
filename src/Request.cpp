#include "Request.h"

Request::Request(int _clientFd, int _serverFd, string _uniqueId, vector<char> _buffer) : clientFd(_clientFd),
                                                                                         serverFd(_serverFd),
                                                                                         uniqueId(_uniqueId),
                                                                                         allContent(_buffer) {
    firstLine = getFirstLineFromBuffer(_buffer);
    type = getTypeFromFirstLine(firstLine);
    headers = getHeadersFromBuffer(_buffer);
    hostName = getHostNameFromHeaders(headers);
    requestIP = getIPFromFd(_clientFd);
    requestTime = getCurrentTime();
    headersMap = getHashMapFromHeaders(headers);
    if (type == CONNECT) {
        int colonIdx = findChar(hostName, ':');
        hostName.resize(colonIdx);
    }
}

Request::Request(const Request &other) {
    this->clientFd = other.clientFd;
    this->serverFd = other.serverFd;
    this->uniqueId = other.uniqueId;
    this->type = other.type;
    this->requestIP = other.requestIP;
    this->requestTime = other.requestTime;
    this->allContent = other.allContent;
    this->firstLine = other.firstLine;
    this->headers = other.headers;
    this->hostName = other.hostName;
}

Request &Request::operator=(const Request &other) {
    if (this->uniqueId != other.uniqueId) {
        this->clientFd = other.clientFd;
        this->serverFd = other.serverFd;
        this->uniqueId = other.uniqueId;
        this->type = other.type;
        this->requestIP = other.requestIP;
        this->requestTime = other.requestTime;
        this->allContent = other.allContent;
        this->firstLine = other.firstLine;
        this->headers = other.headers;
        this->hostName = other.hostName;
    }
    return (*this);
}

vector<char> Request::getFirstLineFromBuffer(vector<char> buffer) {
    int firstLineEndIdx = findAnyCharInSubStr(buffer, strToVector("\r\n"));
    if (firstLineEndIdx == -1) {
        throw MyException("Error: Fail to get head from payloads");
    }
    return vector<char>(buffer.begin(), buffer.begin() + firstLineEndIdx);
}

string Request::getTypeFromFirstLine(vector<char> firstLine) {
    int blankIdx = findChar(firstLine, ' ');
    if (blankIdx == -1) {
        throw MyException("Error: fail to get type of request");
    }
    return string(firstLine.begin(), firstLine.begin() + blankIdx);
}

string Request::getIPFromFd(int clientFd) {
    int status = 0;
    struct sockaddr_in addr_in;
    socklen_t addr_in_len = sizeof(struct sockaddr_in);
    status = getpeername(clientFd, (struct sockaddr *) &addr_in, &addr_in_len);
    if (status == -1) {
        throw MyException("Error: the  address  of  the  peer  connected to the socket clientFd");
    }
    char *ip_address = inet_ntoa(addr_in.sin_addr);
    string res(ip_address);
    return res;
}

vector<char> Request::getHeadersFromBuffer(vector<char> buffer) {
    int firstLineEndIdx = findAnyCharInSubStr(buffer, strToVector("\r\n"));
    if (firstLineEndIdx == -1) {
        throw MyException("Error: Fail to get body from payloads");
    }
    while (firstLineEndIdx < (int) buffer.size() &&
           (buffer[firstLineEndIdx] == '\r' || buffer[firstLineEndIdx] == '\n')) {
        ++firstLineEndIdx;
    }
    if (firstLineEndIdx >= (int) buffer.size()) {
        throw MyException("Error: No body in the payloads");
    }
    int endOfBuffer = findSubStr(buffer, strToVector("\r\n\r\n"));
    return vector<char>(buffer.begin() + firstLineEndIdx, buffer.begin() + endOfBuffer + 4);

}

vector<char> Request::getHostNameFromHeaders(vector<char> headers) {
    vector<char> hostArr = strToVector("Host: ");
    int hostIdx = findSubStr(headers, hostArr);
    if (hostIdx == -1) {
        throw MyException("Fail to find the host name");
    }
    hostIdx += (int) hostArr.size();
    int endOfHostIdx = findAnyCharInSubStr(headers, strToVector("\r\n"), hostIdx);
    return vector<char>(headers.begin() + hostIdx, headers.begin() + endOfHostIdx);
}

unordered_map <string, vector<char>> Request::getHashMapFromHeaders(vector<char> headers) {
    unordered_map <string, vector<char>> hashMap;
    int left = 0, right = 0;
    int preLeft = left;
    while ((left = findSubStr(headers, strToVector(": "), left)) != -1) {
        right = findAnyCharInSubStr(headers, strToVector("\r\n"), left + 1);
        string s(headers.begin() + preLeft, headers.begin() + left);
        vector<char> content(headers.begin() + left + 2, headers.begin() + right);
        hashMap[s] = content;
        left = right + 1;
        while (left < (int) headers.size() && (headers[left] == '\r' || headers[left] == '\n')) {
            ++left;
        }
        preLeft = left;
    }
    return hashMap;
}

string Request::getFirstLineIPTime() {
    return uniqueId + ": \"" + vectorToStr(firstLine)+ "\""+ " from "+requestIP + " @ " + requestTime;
}