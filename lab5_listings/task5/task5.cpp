#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;

DWORD WINAPI Thread1(LPVOID);

int stop;
struct params {
	int num;
	bool* runflg;
};

int main(int argc, char* argv[]) {
	SYSTEMTIME now;
	int thrds;
	if (argc < 3)
		thrds = 2;	// number of threads
	else
		thrds = atoi(argv[1]);
	if (argc < 3)
		stop = 5000;	// time to stop
	else
		stop = atoi(argv[2]);
	DWORD targetThreadId;
	bool runFlag = true;
	__int64 end_time;
	LARGE_INTEGER end_time2;
	// create and set timer
	HANDLE tm1 = CreateWaitableTimer(NULL, false, NULL);
	end_time = -1 * stop * 10000000;
	end_time2.LowPart = (DWORD)(end_time & 0xFFFFFFFF);
	end_time2.HighPart = (LONG)(end_time >> 32);
	SetWaitableTimer(tm1, &end_time2, 0, NULL, NULL, false);
	for (int i = 0; i < thrds; i++) {
		params* param = (params*)malloc(sizeof(params));
		param->num = i;
		param->runflg = &runFlag;
		HANDLE t1 = CreateThread(NULL, 0, Thread1, param, 0, &targetThreadId); // create thread
		CloseHandle(t1);
	}
	GetSystemTime(&now);	// print current time
	cout << "System Time " << now.wHour << " " << now.wMinute << " " << now.wSecond << endl;
	WaitForSingleObject(tm1, INFINITE);
	runFlag = false; // set flag CloseHandle( tm1 );
	GetSystemTime(&now); // print time after setting flag (ending)
	cout << "System Time " << now.wHour << " " << now.wMinute << " " << now.wSecond << endl;
	system("pause");
	return 0;
}

DWORD WINAPI Thread1(LPVOID prm) {
	while (1) {
		params arg = *((params*)prm);
		Sleep(1000);
		cout << arg.num << endl;
		if (*(arg.runflg) == false) // check flag
			break;
	}
	return 0;
}
