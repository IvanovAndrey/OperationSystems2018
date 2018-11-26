#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;

DWORD WINAPI threadHandler(LPVOID);

int main(int argc, char* argv[]) {
	cout << "Program started" << endl;
	HANDLE t;
	int number = 1;
	t = CreateThread(NULL, 0, threadHandler, (LPVOID)number, 0, NULL);
	CloseHandle(t);
	number = 2;
	t = CreateThread(NULL, 0, threadHandler, (LPVOID)number, 0, NULL);
	CloseHandle(t);
	ExitThread( 0 ); // 2 var
	cout << "Program finished" << endl;
	Sleep(300);
	//system("pause");
	//return 0;       // 1 var
}
DWORD WINAPI threadHandler(LPVOID param) {
	int number = (int)param;
	for (;;) {
		Sleep(300);
		cout << number;
		fflush(stdout);
	}
	return 0;
}
