#ifndef ERSSHW2_MEEXCEPTION_H
#define ERSSHW2_MEEXCEPTION_H

#include <exception>
#include <string>

using std::exception;
using std::string;

class MyException : public exception {
private:
    string message;

public:
    MyException() : message(std::string("Exception occurs!") + "\n") {}

    MyException(const string &_message) : message(_message + "\n") {}

    const char *what() const

    noexcept override{return message.c_str();}
};

#endif //ERSSHW2_MEEXCEPTION_H
