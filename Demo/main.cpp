#include "../MSLib/include/MSLib.h"
#pragma comment(lib, "../x64/Release/MSLib.lib")
#include <iostream>

int main()
{
	std::string input;
	while (input != "exit")
	{
		std::cin >> input;
		if (input == "test")
		{
			std::vector<PVOID> threads;
			NTSTATUS status = ms::Thread::GetModuleThreads("ntdll.dll", &threads);
			LogLine("GetModuleThread status - %llx\n", status);
			LogLine("Thread size - %d\n", threads.size());

			status = ms::Thread::SetState(threads, ms::Thread::THREAD_STATE::SUSPEND);
			LogLine("SetState status - %llx\n", status);
		}
	}

	return 0;
}