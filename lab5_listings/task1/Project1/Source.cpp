#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include<TCHAR.H>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
int main(int argc, char** argv[]) {
	printf("Program started\n");
	TCHAR *path = new TCHAR[45];
	strcpy(path, _T("C:\\Windows\\System32\\calc.exe"));
	LPTSTR LpCommandLine = path;            // string wt command in universal form
	STARTUPINFO startupInfo;                        // pointer to structure
	PROCESS_INFORMATION processInfo; 	// info about process
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(startupInfo);
	// creating process
	if (!CreateProcess(NULL, LpCommandLine, NULL, NULL, false, HIGH_PRIORITY_CLASS |
		CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &processInfo)) {
		cout << "Error creating process: " << GetLastError() << endl;
		return -1;
	}
	else {
		cout << "New process Handle: " << processInfo.dwProcessId <<
			" Handle of thread: " << processInfo.dwThreadId << endl <<
			"Successfully created new process!" << endl;
	}
	CloseHandle(processInfo.hThread);	    // закрытие нити
	CloseHandle(processInfo.hProcess);	// закрытие процесса
	cout << "Program finished" << endl;
	getchar();
	return 0;
}
