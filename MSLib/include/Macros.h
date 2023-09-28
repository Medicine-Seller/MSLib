#include <iostream>
#include <iomanip>

#define COL3(X,Y,Z) std::left << std::setw(40) << X << std::left << std::setw(40) << Y << std::left << std::setw(40) << Z
#define COL2(X,Y) std::left << std::setw(40) << X << std::left << std::setw(40) << Y
#define COL1(X) std::left << std::setw(40) << X