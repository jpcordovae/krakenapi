#ifndef KUTILS_H__
#define KUTILS_H__

#include <iostream>
#include <iomanip>
#include <sstream>

class SafePrint: public std::ostringstream
{
public:
    SafePrint() = default;

    ~SafePrint()
    {
        std::lock_guard<std::mutex> guard(_mutexSafePrint);
        std::cout << this->str();
    }

private:
    static std::mutex _mutexSafePrint;
};

std::mutex SafePrint::_mutexSafePrint{};

#endif
