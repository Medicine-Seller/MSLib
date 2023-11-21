#include <iostream>
#include <iomanip>

#ifndef MACROS_H
#define MACROS_H

#define COL3(X,Y,Z) std::left << std::setw(40) << X << std::left << std::setw(40) << Y << std::left << std::setw(40) << Z
#define COL2(X,Y) std::left << std::setw(40) << X << std::left << std::setw(40) << Y
#define COL1(X) std::left << std::setw(40) << X

#define MS_LOG(x) std::cout << std::left << std::setw(40) << std::hex << #x << std::left << std::setw(40) << x << std::endl

#endif