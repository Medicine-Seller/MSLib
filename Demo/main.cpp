#include "../MSLib/include/Input.h"
#pragma comment(lib, "../x64/Release/MSLib.lib")
#include <iostream>

int main()
{
	ms::Input test(INPUT_MOUSE);
	for (int i = 3; i > 0; i--)
	{
		printf("Sending key in %ds...\n", i);
		Sleep(1000);
	}

	for (int i = 3; i > 0; i--)
	{
		printf("Sent\n");
		test.SendMouseClick(1000,1000);
		Sleep(1000);
	}

	return 0;
}