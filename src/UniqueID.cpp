#include "UniqueID.h"

UniqueID::UniqueID(int i) {
    if (i <= 0) {
        digits = 1;
    } else {
        digits = i;
    }
    string s(i, '0');
    unique_id = s;
}

string UniqueID::updateID() {
    if (unique_id[0] == '9') {
        ++digits;
        string s(digits, '0');
        unique_id = s;
    } else {
        for (int i = digits - 1; i >= 0; --i) {
            if (unique_id[i] < '9') {
                ++unique_id[i];
                return unique_id;
            } else {
                unique_id[i] = '0';
            }
        }
    }
    return unique_id;
}

string UniqueID::getUniqueID() {
    return unique_id;
}

