#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <iostream>
#define MAX_LEN 200
using namespace std;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

int main(int argc, char* argv[]) {
	const char* frd = "C:\\Users\\Lady of the flowers\\Desktop\\ÎÑè\\temp.txt";
	FILE *f = fopen(frd, "r");
	if (f == NULL) {
		cout << "Coudn't open file" << endl;
		system("pause");
		return 1;
	}
	for (int i = 0; i < 2; i++) {
		char* execString = (char*)calloc(MAX_LEN, sizeof(char));  // memory allocation
		fgets(execString, MAX_LEN, f);                              // reading string from file
		execString[strlen(execString) - 1] = '\0';
		STARTUPINFO startupInfo;
		ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
		startupInfo.cb = sizeof(startupInfo);
		PROCESS_INFORMATION processInfo;
		cout << execString << endl;
		if (!CreateProcess(NULL, execString, NULL, NULL, false, 0, NULL, NULL, &startupInfo, &processInfo)) {
			cout << "Error creating process: " << GetLastError() << endl;
			return -1;
		}
		else
			cout << "Process successfully created!" << endl;
		free(execString);
		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);
	}
	system("pause");
	return 0;
}
