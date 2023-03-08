#include <iostream>
#include <string.h>
#include <Windows.h>

char ShaderName[256] = { 0 };
int main(int argc, char* argv[])
{
	if (argc < 2)
		return 1;

	for (int i = 1; i < argc; ++i)
	{
		std::cout << argv[i] << std::endl;
	}
	memcpy(ShaderName, argv[1], strlen(argv[1]));

	// Find Eggy Windows
	HWND hwnd = NULL;
	hwnd = ::FindWindowExA(NULL, hwnd, NULL, "Eggy");
	if (hwnd)
	{
		std::cout << "find Eggy!!!" << std::endl;
		::SendMessageA(hwnd, WM_COPYDATA, 1, (LPARAM)(LPVOID)&ShaderName);
	}
	return 0;
}