#define NAMESPACE_BEGIN namespace ms {
#define NAMESPACE_END }

#define ENABLE_LOGGING 1

#define COL3(X,Y,Z) std::left << std::setw(40) << X << std::left << std::setw(40) << Y << std::left << std::setw(40) << Z
#define COL2(X,Y) std::left << std::setw(40) << X << std::left << std::setw(40) << Y
#define COL1(X) std::left << std::setw(40) << X

#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <Psapi.h>
#include <sstream>
#include <TlHelp32.h>
#include <iomanip>
#include <cstdio>

NAMESPACE_BEGIN

#include "Util.h"
#include "Registers.h"
#include "Console.h"
#include "Scan.h"
#include "Patch.h"
#include "Detour.h"
#include "VirtualProtect.h"
#include "Thread.h"

NAMESPACE_END
