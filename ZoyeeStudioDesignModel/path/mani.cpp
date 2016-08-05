#include <Windows.h>
#include <iostream>

#define PythonPath "E:\\Python27\\python.exe"
#define PythonFile "x:\\Hello.py"

int main()
{
	char sz[128] = "";
	sprintf(sz, "%s %s", PythonPath, PythonFile);
	system(sz);
}