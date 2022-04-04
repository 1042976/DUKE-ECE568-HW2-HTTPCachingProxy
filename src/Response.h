#ifndef ERSSHW2_RESPONSE_H
#define ERSSHW2_RESPONSE_H

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

class Response {
private:
    string responseTime;
    string errorCondition;
    string uniqueId;
    int bufferLen;
    vector<char> allContent;
    vector<char> firstLine;
    vector<char> headers;
    unordered_map <string, vector<char>> headersMap;
    vector<char> cacheControl;
public:

    Response();

    Response(string _uniqueId, int _bufferLen, vector<char> _buffer);

    Response(const Response &other);

    Response &operator=(const Response &other);

    string getErrorCondition();

    string getUniqueId();

    int getBufferLen();

    vector<char> getAllContent();

    vector<char> getFirstLine();

    vector<char> getHeaders();

    unordered_map <string, vector<char>> getHeadersMap();

    bool isChunked();
    bool lessThanContentLength();
    bool hasContentLength();
    void addContent(int _bufferLen, vector<char> Content);

    //For cache control
    vector<char> getCacheControl();
    vector<char> getMaxAge();
    vector<char> getSMaxAge();
    vector<char> getAge();
    bool isNoStore();
    bool isPublic();
    bool isPrivate();
    bool mustRevalidate();
    bool proxyRevalidate();
    bool isNoCache();
    vector<char> getDate();
    vector<char> getExpires();
    vector<char> getLastModified();
    vector<char> getEtag();
    vector<char> getExpirationTime();
    static vector<char> getFirstLineFromBuffer(vector<char> buffer);

    static string getErrorConditionFromFirstLine(vector<char> firstLine);

    static vector<char> getHeadersFromBuffer(vector<char> buffer);

    static unordered_map <string, vector<char>> getHashMapFromHeaders(vector<char> headers);


};

#endif //ERSSHW2_RESPONSE_H
