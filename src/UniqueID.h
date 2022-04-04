#ifndef ERSSHW2_UNIQUEID_H
#define ERSSHW2_UNIQUEID_H

#include <cstdlib>
#include <cstdio>
#include "string"
#include "cstring"

using namespace std;

class UniqueID {
private:
    int digits;
    string unique_id;

public:
    UniqueID(int i);

    string updateID();

    string getUniqueID();

    //-------------------
};

#endif //ERSSHW2_UNIQUEID_H
