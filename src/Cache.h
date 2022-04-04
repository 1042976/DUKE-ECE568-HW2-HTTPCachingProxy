
#ifndef ERSSHW2_CACHE_H
#define ERSSHW2_CACHE_H
#include "Response.h"
#include "Request.h"
#include "map"
class Cache{
private:
    int maxCacheSize;
    unordered_map<string, string> requestToUniqueId; //first line and unique id
    map<string, Response> uniqueIdToResponse;
public:
    Cache();
    Cache(int _maxCacheSize);
    bool isFull();
    void toCache(string firstLineOfRequest, Response rsToCache);

    string getRequestCacheId(string firstLineOfRequest);

    Response getResponseThroughId(string uniqueId);

    void logNotCacheable(string uniqueId, string reason);

    void logCachedExpiration(string uniqueId,string expires);

    void logCachedRevalidation(string uniqueId);

    void logNotInCache(string uniqueId);

    void logInCacheButExpired(string uniqueId, string expires);

    void logInCacheRevalidation(string uniqueId);

    void logInCacheValid(string uniqueId);
};
#endif //ERSSHW2_CACHE_H
