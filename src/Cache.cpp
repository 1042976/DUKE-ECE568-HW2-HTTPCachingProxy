#include "Cache.h"

Cache::Cache():maxCacheSize(MAX_CACHE_SIZE) {}

Cache::Cache(int _maxCacheSize):maxCacheSize(MAX_CACHE_SIZE) {}

bool Cache::isFull() {
    return (int)uniqueIdToResponse.size() >= maxCacheSize;
}

string Cache::getRequestCacheId(string firstLineOfRequest) {
    if(requestToUniqueId.count(firstLineOfRequest) == 1){
        return requestToUniqueId[firstLineOfRequest];
    }else{
        return "";
    }
}


void Cache::toCache(string firstLineOfRequest, Response rsToCache) {
    lock_guard<mutex> lck(m);
    string lastId = getRequestCacheId(firstLineOfRequest);
    string newId = rsToCache.getUniqueId();
    if(lastId != "" || !isFull()){
        requestToUniqueId[firstLineOfRequest] = newId;
        uniqueIdToResponse.erase(lastId);
        uniqueIdToResponse[newId] = rsToCache;
    }else{
        auto ita = uniqueIdToResponse.begin();
        string smallestId = ita->first;
        uniqueIdToResponse.erase(ita);
        uniqueIdToResponse[newId] = rsToCache;
        auto itb = requestToUniqueId.begin();
        while(itb != requestToUniqueId.end()){
            if(itb->second == smallestId){
                break;
            }else{
                ++itb;
            }
        }
        if(itb == requestToUniqueId.end()){
            perror("Error: Two hash maps in the cache are not consistent");
            exit(EXIT_FAILURE);
        }
        requestToUniqueId.erase(itb);
        requestToUniqueId[firstLineOfRequest] = newId;
    }
    if(rsToCache.isNoCache()){
        logCachedRevalidation(newId);
        return;
    }
    vector<char> expirationTime = rsToCache.getExpirationTime();
    if(!expirationTime.empty()){
        logCachedExpiration(newId, vectorToStr(expirationTime));
    }else{
        logCachedRevalidation(newId);
    }
}

Response Cache::getResponseThroughId(string uniqueId) {
    if(uniqueIdToResponse.count(uniqueId) == 0){
        perror("Error: use uncached uniqueId to get response!");
        exit(EXIT_FAILURE);
    }
    Response rs = uniqueIdToResponse[uniqueId];
    return rs;
}

void Cache::logNotCacheable(string uniqueId, string reason) {
    cout << addColon(uniqueId) << "not cacheable because " << reason << endl;
    PROXYLOG << addColon(uniqueId) << "not cacheable because " << reason << endl;
}

void Cache::logCachedExpiration(string uniqueId, string expires) {
    cout << addColon(uniqueId) << "cached, expires at " << expires << endl;
    PROXYLOG << addColon(uniqueId) << "cached, expires at " << expires << endl;
}

void Cache::logCachedRevalidation(string uniqueId) {
    cout << addColon(uniqueId) << "cached, but requires re-validation" << endl;
    PROXYLOG << addColon(uniqueId) << "cached, but requires re-validation" << endl;
}

void Cache::logNotInCache(string uniqueId) {
    cout << addColon(uniqueId) << "not in cache" << endl;
    PROXYLOG << addColon(uniqueId) << "not in cache" << endl;
}

void Cache::logInCacheValid(string uniqueId) {
    cout << addColon(uniqueId) << "in cache, valid" << endl;
    PROXYLOG << addColon(uniqueId) << "in cache, valid" << endl;
}

void Cache::logInCacheButExpired(string uniqueId, string expires) {
    cout << addColon(uniqueId) << "in cache, but expired at " << expires << endl;
    PROXYLOG << addColon(uniqueId) << "in cache, but expired at " << expires << endl;
}

void Cache::logInCacheRevalidation(string uniqueId) {
    cout << addColon(uniqueId) << "in cache, requires validation" << endl;
    PROXYLOG << addColon(uniqueId) << "in cache, requires validation" << endl;
}