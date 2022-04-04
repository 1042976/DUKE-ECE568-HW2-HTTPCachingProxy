#ifndef ERSSHW2_DATA_H
#define ERSSHW2_DATA_H

#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <climits>
#include <iostream>
#include <fstream>
#include <mutex>
#include <ctime>
#include <unordered_map>
#include "MyException.h"

using namespace std;

extern string CONTENT_LENGTH;
extern string TRANSFER_ENCODING;
extern string MYPORT;
extern int MAX_CACHE_SIZE;
extern int BACKLOG;
extern int RECV_SEND_PAYLOAD_LEN;
extern string GET;
extern string POST;
extern string CONNECT;
extern std::mutex m;
extern ofstream PROXYLOG;

//For cache
extern string CACHE_CONTROL;
extern string MAX_AGE;
extern string S_MAX_AGE;
extern string KEY_AGE;
extern string NO_STORE;
extern string PUBLIC_CONTROL;
extern string PRIVATE_CONTROL;
extern string MUST_REVALIDATE;
extern string PROXY_REVALIDATE;
extern string NO_CACHE;
extern string KEY_DATE;
extern string KEY_EXPIRES;

//For re-validation
extern int NOTINCACHE;
extern int INCACHEBUTEXPIRED;
extern int INCACHEREQUIRESVALIDATION;
extern int INCACHEVALID;
extern int UNKNOWN;
extern string LAST_MODIFIED;
extern string ETAG;
extern string IF_MODIFIED_SINCE;
extern string IF_NONE_MATCH;

//For log
extern string NO_ID;

void printVector(vector<char> arr);

void printHashMap(unordered_map <string, vector<char>> hashMap);

vector<char> strToVector(string str);

string vectorToStr(vector<char> arr);

const char *vectorToConstChar(vector<char> arr);

char *vectorToChar(vector<char> arr);

vector<char> constCharToVector(const char *arr);

int findSubStr(const vector<char> &A, vector<char> target);

int findSubStr(const vector<char> &A, vector<char> target, size_t left);

int findAnyCharInSubStr(const vector<char> &A, vector<char> target);

int findAnyCharInSubStr(const vector<char> &A, vector<char> target, size_t left);

int findChar(const vector<char> &A, char target);

int findLastChar(const vector<char> &A, char target);

int findChar(const vector<char> &A, char target, size_t left);

vector<char> getSubStr(const vector<char> &A, size_t i);

vector<char> getSubStr(const vector<char> &A, size_t i, size_t len);

string getCurrentTime();

string getTimeAfterAddingSeconds(string t, string dt);

void standardizeVector(vector<char> &buffer);

void setZeroInTheEnd(vector<char> &buffer);

string addQuotationMarks(string str);

string addColon(string str);

vector<char> combineVector(vector<char> arr1, vector<char> arr2);

string secondsAddition(string t1, string t2);

string secondsMinus(string t1, string t2);

bool compareUTC(string t1, string t2);

vector<char> buildHeaders(vector<vector<char> >& lines);

void logNote(string UniqueId, string message);

void logWarning(string UniqueId, string message);

void logError(string UniqueId, string message);
#endif //ERSSHW2_DATA_H
