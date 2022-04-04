#ifndef ERSSHW2_REQUEST_H
#define ERSSHW2_REQUEST_H

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <unistd.h>
#include "MyException.h"
#include "MyData.h"

class Request {
private:
    int clientFd;
    int serverFd;
    string uniqueId;
    string type;
    string requestIP;
    string requestTime;
    vector<char> allContent;
    vector<char> firstLine;
    vector<char> headers;
    vector<char> hostName;
    unordered_map <string, vector<char>> headersMap;
public:
    Request(int clientFd, int serverFd, string _uniqueId, vector<char> _buffer);

    Request(const Request &other);

    Request &operator=(const Request &other);

    int getClientFd() { return clientFd; }

    int getServerFd() { return serverFd; }

    string getFirstLineIPTime();

    string getType() { return type; }

    string getUniqueId() { return uniqueId; }

    string getRequestIP() { return requestIP; }

    string getRequestTime() { return requestTime; }

    string getUniqueIDWithColon(){return uniqueId+": ";}

    vector<char> getFirstLine() { return firstLine; }

    vector<char> getHeaders() { return headers; }

    vector<char> getHostName() { return hostName; }

    vector<char> getAllContent() { return allContent; }

    static string getTypeFromFirstLine(vector<char> firstLine);

    static string getIPFromFd(int clientFd);

    static vector<char> getFirstLineFromBuffer(vector<char> buffer);

    static vector<char> getHeadersFromBuffer(vector<char> buffer);

    static vector<char> getHostNameFromHeaders(vector<char> headers);

    static unordered_map <string, vector<char>> getHashMapFromHeaders(vector<char> headers);

    //---------------------------------

};

#endif //ERSSHW2_REQUEST_H