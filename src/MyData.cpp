#include "MyData.h"

string CONTENT_LENGTH = "Content-Length";
string TRANSFER_ENCODING = "Transfer-Encoding";
//----------
string MYPORT = "12345";
int MAX_CACHE_SIZE = 10;
int BACKLOG = 100;
int RECV_SEND_PAYLOAD_LEN = 100000;
string GET = "GET";
string POST = "POST";
string CONNECT = "CONNECT";

std::mutex m;
std::ofstream PROXYLOG("/var/log/erss/proxy.log");

//For cache
string CACHE_CONTROL = "Cache-Control";
string MAX_AGE = "max-age";
string S_MAX_AGE = "s-maxage";
string KEY_AGE = "Age";
string NO_STORE = "no-store";
string PUBLIC_CONTROL = "public";
string PRIVATE_CONTROL = "private";
string MUST_REVALIDATE = "must-revalidate";
string PROXY_REVALIDATE = "proxy-revalidate";
string NO_CACHE = "no-cache";
string KEY_DATE = "Date";
string KEY_EXPIRES = "Expires";

//For re-validation
int NOTINCACHE = 0;
int INCACHEBUTEXPIRED = 1;
int INCACHEREQUIRESVALIDATION = 2;
int INCACHEVALID = 3;
int UNKNOWN = 4;
string LAST_MODIFIED = "Last-Modified";
string ETAG = "ETag";
string IF_MODIFIED_SINCE = "If-Modified-Since";
string IF_NONE_MATCH = "If-None-Match";


//For lod
string NO_ID = "no_id";

vector<char> strToVector(string str) {
    return vector<char>(str.begin(), str.end());
}

string vectorToStr(vector<char> arr) {
    return string(arr.begin(), arr.end());
}

int findSubStr(const vector<char> &A, vector<char> target) {
    if (target.empty()) {
        return 0;
    }
    if (A.empty()) {
        return -1;
    }
    auto it = A.begin();
    while (it != A.end()) {
        it = find(it, A.end(), target[0]);
        if (it == A.end() || (int) (A.end() - it) < (int) target.size()) {
            return -1;
        } else {
            auto endIt = it + target.size();
            vector<char> tmp(it, endIt);
            if (tmp == target) {
                return it - A.begin();
            }
        }
        ++it;
    }
    return -1;
}

int findSubStr(const vector<char> &A, vector<char> target, size_t left) {
    if (left >= A.size()) {
        return -1;
    }
    vector<char> tmp(A.begin() + left, A.end());
    int k = findSubStr(tmp, target);
    if (k < 0) {
        return -1;
    } else {
        return left + k;
    }
}

int findAnyCharInSubStr(const vector<char> &A, vector<char> target) {
    int min_index = INT_MAX;
    for (size_t i = 0; i < target.size(); ++i) {
        int cur = findChar(A, target[i]);
        if (cur == -1) {
            continue;
        }
        min_index = min(min_index, cur);
    }
    return min_index == INT_MAX ? -1 : min_index;
}

int findAnyCharInSubStr(const vector<char> &A, vector<char> target, size_t left) {
    if (left >= A.size()) {
        return -1;
    }
    vector<char> tmp(A.begin() + left, A.end());
    int targetPos = findAnyCharInSubStr(tmp, target);
    return targetPos < 0 ? -1 : left + targetPos;
}

int findChar(const vector<char> &A, char target) {
    for (size_t i = 0; i < A.size(); ++i) {
        if (A[i] == target) {
            return i;
        }
    }
    return -1;
}

int findChar(const vector<char> &A, char target, size_t left) {
    if (left >= A.size()) {
        return -1;
    }
    vector<char> tmp(A.begin() + left, A.end());
    int k = findChar(tmp, target);
    return k < 0 ? -1 : left + k;
}

int findLastChar(const vector<char> &A, char target){
    for(int i = A.size()-1; i >= 0; --i){
        if(A[i] == target){
            return i;
        }
    }
    return -1;
}

vector<char> getSubStr(const vector<char> &A, size_t i) {
    if (i >= A.size()) {
        return vector < char > {};
    } else {
        return vector<char>(A.begin() + i, A.end());
    }
}

vector<char> getSubStr(const vector<char> &A, size_t i, size_t len) {
    if (i + len - 1 >= A.size()) {
        return getSubStr(A, i);
    }
    return vector<char>(A.begin() + i, A.begin() + i + len);
}

void printVector(vector<char> arr) {
    if (arr.empty()) {
        cout << endl;
        return;
    }
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << arr[i];
    }
    cout << endl;
}

const char *vectorToConstChar(vector<char> arr) {
    return vectorToStr(arr).c_str();
}

vector<char> constCharToVector(const char *arr) {
    vector<char> res;
    const char *p = arr;
    while (*p != '\0') {
        res.emplace_back(*p);
        ++p;
    }
    return res;
}

vector<char> charToVector(char *arr) {
    char *p = arr;
    vector<char> res;
    while (*p != '\0') {
        res.emplace_back(*p);
    }
    return res;
}

char *vectorToChar(vector<char> arr) {
    return arr.data();
}

string getCurrentTime() {
    time_t now = time(NULL);
    tm *gmtm = gmtime(&now);
    char *dt = asctime(gmtm);
    string dt_str = "";
    char *p = dt;
    while (*p != '\n') {
        dt_str.push_back(*p);
        ++p;
    }
    return dt_str;
}

string getTimeAfterAddingSeconds(string t, string delta){
    struct tm a;
    strptime(t.c_str(), "%a, %d %b %Y %H:%M:%S", &a);
    //a.tm_sec += stoll(delta);
    time_t b = mktime(&a)+ stoll(delta)-18000;
    tm *gmtm = gmtime(&b);
    char *dt = asctime(gmtm);
    string dt_str = "";
    char *p = dt;
    while (*p != '\n') {
        dt_str.push_back(*p);
        ++p;
    }
    return dt_str;
}

void printHashMap(unordered_map <string, vector<char>> hashMap) {
    unordered_map < string, vector < char > > ::iterator
    it = hashMap.begin();
    while (it != hashMap.end()) {
        cout << it->first << ": ";
        printVector(it->second);
        ++it;
    }
}

void standardizeVector(vector<char> &buffer) {
    size_t actual_len = 0;
    while (actual_len < buffer.size() && buffer[actual_len] != '\0') {
        ++actual_len;
    }
    buffer.resize(actual_len);
}

void setZeroInTheEnd(vector<char> &buffer) {
    int firstZeroIdx = findChar(buffer, '\0');
    if (firstZeroIdx == -1) {
        buffer.emplace_back('\0');
    }
}

string addQuotationMarks(string str){
    return "\""+str+"\"";
}

string addColon(string str){
    return str + ": ";
}

vector<char> combineVector(vector<char> arr1, vector<char> arr2){
    vector<char> res(arr1.size()+arr2.size(), '\0');
    for(size_t i = 0; i < arr1.size(); ++i){
        res[i] = arr1[i];
    }
    size_t j = 0;
    for(size_t i = arr1.size(); i < arr1.size()+arr2.size(); ++i){
        res[i] = arr2[j++];
    }
    return res;
}

string secondsAddition(string t1, string t2){
    auto _t1 = stoll(t1);
    auto _t2 = stoll(t2);
    return to_string(_t1+_t2);
}

string secondsMinus(string t1, string t2){
    auto _t1 = stoll(t1);
    auto _t2 = stoll(t2);
    return to_string(_t1-_t2);
}

bool compareUTC(string t1, string t2){
    struct tm a1, a2;
    strptime(t1.c_str(), "%a %b %d %H:%M:%S %Y", &a1);
    strptime(t2.c_str(), "%a %b %d %H:%M:%S %Y", &a2);
    time_t _t1 = mktime(&a1);
    time_t _t2 = mktime(&a2);
    return difftime(_t1,_t2) > 0.0;
}

vector<char> buildHeaders(vector<vector<char> >& lines){
    if(lines.empty()){
        return vector<char>{};
    }
    vector<char> res;
    for(int i = 0; i < (int)lines.size()-1; ++i){
        res = combineVector(res, lines[i]);
        res = combineVector(res, strToVector("\r\n"));
    }
    res = combineVector(res, lines.back());
    res = combineVector(res, strToVector("\r\n\r\n"));
    return res;
}

void logNote(string UniqueId, string message){
    cout << addColon(UniqueId) << "NOTE " << message << endl;
    PROXYLOG << addColon(UniqueId) << "NOTE " << message << endl;
}

void logWarning(string UniqueId, string message){
    cout << addColon(UniqueId) << "WARNING " << message << endl;
    PROXYLOG << addColon(UniqueId) << "WARNING " << message << endl;
}

void logError(string UniqueId, string message){
    cout << addColon(UniqueId) << "ERROR " << message << endl;
    PROXYLOG << addColon(UniqueId) << "ERROR " << message << endl;
}