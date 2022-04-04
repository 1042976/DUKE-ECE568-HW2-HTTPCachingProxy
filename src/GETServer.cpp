#include "GETServer.h"

int GETServer::checkCache() {
    lock_guard<mutex> lck(m);
    int status = UNKNOWN;
    string newId = r.getUniqueId();
    string lastId = myCache->getRequestCacheId(vectorToStr(r.getFirstLine()));
    if(lastId.empty()){
        myCache->logNotInCache(newId);
        return NOTINCACHE;
    }
    Response cachedRs = myCache->getResponseThroughId(lastId);
    //build validation headers
    vector<vector<char> > validationComponents;
    validationComponents.emplace_back(r.getFirstLine());
    vector<char> lastModified = cachedRs.getLastModified();
    vector<char> etag = cachedRs.getEtag();
    if(!lastModified.empty()){
        validationComponents.emplace_back(lastModified);
    }
    if(!etag.empty()){
        validationComponents.emplace_back(etag);
    }
    vector<char> validationHeaders = buildHeaders(validationComponents);
    bool canValidate = !(lastModified.empty()&&etag.empty());
    cout << canValidate << endl;

    //no-cache: must validate each time
    if(cachedRs.isNoCache()){
        myCache->logInCacheRevalidation(newId);
        if(canValidate){
            status = INCACHEREQUIRESVALIDATION;
        }else{
            return UNKNOWN;
        }
    }
    vector<char> expirationTime = cachedRs.getExpirationTime();
    if(!expirationTime.empty()){
        string currentTime = getCurrentTime();
        bool isExpired = compareUTC(currentTime, vectorToStr(expirationTime));
        if(isExpired){
            myCache->logInCacheButExpired(newId, vectorToStr(expirationTime));
            if(cachedRs.mustRevalidate() || cachedRs.proxyRevalidate()){
                if(canValidate){
                    status =  INCACHEREQUIRESVALIDATION;
                }else{
                    return UNKNOWN;
                }
            }else{
                return UNKNOWN;
            }
        }else{
            myCache->logInCacheValid(newId);
            setResponse(cachedRs);
            sendResponseToClient();
            return INCACHEVALID;
        }
    }else{
        myCache->logInCacheRevalidation(newId);
        if(canValidate){
            status = INCACHEREQUIRESVALIDATION;
        }else{
            return UNKNOWN;
        }
    }
    if(status == INCACHEREQUIRESVALIDATION){
        Request validationRequest(r.getClientFd(), r.getServerFd(), r.getUniqueId(), validationHeaders);
        Request tmp = r;
        r = validationRequest;
        getResponse();
        r = tmp;
        if(rs.getErrorCondition() == "304"){
            setResponse(cachedRs);
            sendResponseToClient();
        }else if(rs.getErrorCondition() == "200"){
            sendResponseToClient();
            updateCache();
        }else{
            return UNKNOWN;
        }
    }
    return status;
}

void GETServer::updateCache() {
    if(rs.getErrorCondition() != "200"){
        return;
    }
    if(rs.isPrivate()){
        myCache->logNotCacheable(r.getUniqueId(),"it is private");
        return;
    }
    if(rs.isNoStore()){
        myCache->logNotCacheable(r.getUniqueId(),"it contains \"no-store\"");
        return;
    }
    myCache->toCache(vectorToStr(r.getFirstLine()), rs);
}

void GETServer::getResponse() {
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

void GETServer::sendResponseToClient() {
    int status = 0;
    vector<char> responseInfo = rs.getAllContent();
    setZeroInTheEnd(responseInfo);
    status = send(r.getClientFd(), responseInfo.data(), rs.getBufferLen(), 0);
    if(status == -1){
        logError(r.getUniqueId(), "Fail to send response to the client!");
        throw MyException();
    }
    logResponding(vectorToStr(rs.getFirstLine()));
}

void GETServer::closeFd() {
    close(r.getClientFd());
    close(clientSocket.getFd());
}

