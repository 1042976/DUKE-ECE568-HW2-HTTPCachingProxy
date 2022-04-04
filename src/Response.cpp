#include "Response.h"
Response::Response() {}
Response::Response(string _uniqueId, int _bufferLen, vector<char> _buffer) : uniqueId(_uniqueId), bufferLen(_bufferLen),allContent(_buffer) {
    responseTime = getCurrentTime();
    firstLine = getFirstLineFromBuffer(_buffer);
    errorCondition = getErrorConditionFromFirstLine(firstLine);
    headers = getHeadersFromBuffer(_buffer);
    headersMap = getHashMapFromHeaders(headers);
    cacheControl = getCacheControl();
}
Response::Response(const Response &other) {
    this->responseTime = other.responseTime;
    this->errorCondition = other.errorCondition;
    this->uniqueId = other.uniqueId;
    this->bufferLen = other.bufferLen;
    this->allContent = other.allContent;
    this->firstLine = other.firstLine;
    this->headers = other.headers;
    this->headersMap = other.headersMap;
    this->cacheControl = other.cacheControl;
}

Response &Response::operator=(const Response &other) {
    if (this->uniqueId != other.uniqueId) {
        this->responseTime = other.responseTime;
        this->errorCondition = other.errorCondition;
        this->uniqueId = other.uniqueId;
        this->bufferLen = other.bufferLen;
        this->allContent = other.allContent;
        this->firstLine = other.firstLine;
        this->headers = other.headers;
        this->headersMap = other.headersMap;
        this->cacheControl = other.cacheControl;
    }
    return (*this);
}
vector<char> Response::getFirstLine() {
    return firstLine;
}

vector<char> Response::getAllContent() {
    return allContent;
}

string Response::getErrorCondition() {
    return errorCondition;
}

int Response::getBufferLen() {
    return bufferLen;
}
vector<char> Response::getHeaders() {
    return headers;
}

unordered_map <string, vector<char>> Response::getHeadersMap() {
    return headersMap;
}

bool Response::isChunked() {
    return headersMap.count(TRANSFER_ENCODING) == 1 && headersMap[TRANSFER_ENCODING] == strToVector("chunked");
}

bool Response::hasContentLength(){
    return headersMap.count(CONTENT_LENGTH) == 1;
}

void Response::addContent(int _bufferLen, vector<char> newContent) {
    bufferLen += _bufferLen;
    allContent = combineVector(allContent, newContent);
}

bool Response::lessThanContentLength() {
    if(isChunked()){
        return false;
    }
    if(headersMap.count(CONTENT_LENGTH) == 1){
        int totalLen = headers.size() + stoi(vectorToStr(headersMap[CONTENT_LENGTH]));
        return bufferLen < totalLen;
    }else{
        return false;
    }
}

string Response::getUniqueId() {
    return uniqueId;
}

vector<char> Response::getFirstLineFromBuffer(vector<char> buffer) {
    int firstLineEndIdx = findAnyCharInSubStr(buffer, strToVector("\r\n"));
    if (firstLineEndIdx == -1) {
        throw MyException("Error: Fail to get head from payloads");
    }
    return vector<char>(buffer.begin(), buffer.begin() + firstLineEndIdx);
}

string Response::getErrorConditionFromFirstLine(vector<char> firstLine) {
    string error_message = "Error: fail to find the error condition of the response!";
    int blankIdx = findChar(firstLine, ' ');
    if (blankIdx == -1) {
        throw MyException(error_message);
    }
    int secondBlankIdx = findChar(firstLine, ' ', blankIdx + 1);
    if (secondBlankIdx == -1) {
        throw MyException(error_message);
    }
    return string(firstLine.begin() + blankIdx + 1, firstLine.begin() + secondBlankIdx);
}

vector<char> Response::getHeadersFromBuffer(vector<char> buffer) {
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

unordered_map <string, vector<char>> Response::getHashMapFromHeaders(vector<char> headers) {
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

//For cache
vector<char> Response::getCacheControl() {
    if(headersMap.count(CACHE_CONTROL) == 1){
        return headersMap[CACHE_CONTROL];
    }else{
        return vector<char>{};
    }
}

vector<char> Response::getMaxAge() {
    int k = findSubStr(cacheControl, strToVector(MAX_AGE));
    if(k == -1){
        return vector<char>{};
    }else{
        k += (MAX_AGE.length()+1);
        int endIdx = k;
        while(endIdx < (int)cacheControl.size() && isdigit(cacheControl[endIdx])){
            ++endIdx;
        }
        return vector<char>(cacheControl.begin()+k, cacheControl.begin()+endIdx);
    }
}
vector<char> Response::getSMaxAge() {
    int k = findSubStr(cacheControl, strToVector(S_MAX_AGE));
    if(k == -1){
        return vector<char>{};
    }else{
        k += (S_MAX_AGE.length()+1);
        int endIdx = k;
        while(endIdx < (int)cacheControl.size() && isdigit(cacheControl[endIdx])){
            ++endIdx;
        }
        return vector<char>(cacheControl.begin()+k, cacheControl.begin()+endIdx);
    }
}

vector<char> Response::getAge() {
    if(headersMap.count(KEY_AGE) == 0){
        return vector<char>{};
    }
    vector<char> age  = headersMap[KEY_AGE];
    return age;
}

vector<char> Response::getExpires() {
    int startIdx = findSubStr(headers, strToVector(KEY_EXPIRES));
    if (startIdx == -1) {
        return vector < char > {};
    }
    if(headersMap.count(KEY_EXPIRES) == 0){
        return vector<char>{};
    }
    vector<char> tmp = headersMap[KEY_EXPIRES];
    int k = findLastChar(tmp, ' ');
    if(k == -1){
        perror("Error: Date of response does not have a correct format!");
        exit(EXIT_FAILURE);
    }
    return vector<char>(tmp.begin(), tmp.begin()+k);
}

vector<char> Response::getLastModified() {
    if(headersMap.count(LAST_MODIFIED) == 0){
        return vector<char>{};
    }
    vector<char> res  = headersMap[LAST_MODIFIED];
    return res;
}

vector<char> Response::getEtag() {
    if(headersMap.count(ETAG) == 0){
        return vector<char>{};
    }
    vector<char> res = headersMap[ETAG];
    return res;
}

bool Response::isNoStore() {
    int k = findSubStr(cacheControl, strToVector(NO_STORE));
    return k != -1;
}

bool Response::isPublic() {
    int k = findSubStr(cacheControl, strToVector(PUBLIC_CONTROL));
    return k != -1;
}

bool Response::isPrivate() {
    int k = findSubStr(cacheControl, strToVector(PRIVATE_CONTROL));
    return k != -1;
}

bool Response::mustRevalidate() {
    int k = findSubStr(cacheControl, strToVector(MUST_REVALIDATE));
    return k != -1;
}

bool Response::proxyRevalidate() {
    int k = findSubStr(cacheControl, strToVector(PROXY_REVALIDATE));
    return k != -1;
}

bool Response::isNoCache() {
    int k = findSubStr(cacheControl, strToVector(NO_CACHE));
    return k != -1;
}

vector<char> Response::getDate() {
    if(headersMap.count(KEY_DATE) == 0){
        return vector<char>{};
    }
    vector<char> tmp  = headersMap[KEY_DATE];
    int k = findLastChar(tmp, ' ');
    if(k == -1){
        perror("Error: Date of response does not have a correct format!");
        exit(EXIT_FAILURE);
    }
    return vector<char>(tmp.begin(), tmp.begin()+k);
}


vector<char> Response::getExpirationTime() {
    string maxAge = vectorToStr(getMaxAge());
    string sMaxAge = vectorToStr(getSMaxAge());
    string age = vectorToStr(getAge());
    string createdDate = vectorToStr(getDate());
    string expires = vectorToStr(getExpires());
    vector<char> expirationTime;
    //priority: s-maxage > max-age > expires
    if(!sMaxAge.empty()){
        if(sMaxAge == "0"){
            return vector<char>{};
        }else{
            if(!createdDate.empty()){
                expirationTime = strToVector(getTimeAfterAddingSeconds(createdDate, sMaxAge));
            }else if(!age.empty()){
                string deltaAge = secondsMinus(sMaxAge, age);
                expirationTime = strToVector(getTimeAfterAddingSeconds(responseTime, deltaAge));
            }else{
                return vector<char>{};
            }
        }
    }else if(!maxAge.empty()){
        if(maxAge == "0"){
            return vector<char>{};
        }else {
            if (!createdDate.empty()) {
                expirationTime = strToVector(getTimeAfterAddingSeconds(createdDate, maxAge));
            }else if(!age.empty()){
                string deltaAge = secondsMinus(maxAge, age);
                expirationTime = strToVector(getTimeAfterAddingSeconds(responseTime, deltaAge));
            }else{
                return vector<char>{};
            }
        }
    }else if(!expires.empty()){
        expirationTime = strToVector(expires);
    }
    return expirationTime;
}