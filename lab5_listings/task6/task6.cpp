#define _WIN32_WINNT 0x0500
#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;

DWORD WINAPI Thread1(LPVOID);
int stop;
int sleep = 10000;
struct params {
	int num;
	bool* runflg;
};

long long counters[7] = { 0, 0, 0, 0, 0, 0, 0 }; // counter for threads
int priority[7] = { THREAD_PRIORITY_IDLE,THREAD_PRIORITY_LOWEST,
				   THREAD_PRIORITY_BELOW_NORMAL,THREAD_PRIORITY_NORMAL,
				   THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST,
				   THREAD_PRIORITY_TIME_CRITICAL }; //massive of priorities in ascending order

int main(int argc, char* argv[]) { // on the command line set the thread life time
	stop = 10;
	DWORD targetThreadId;
	bool runFlag = true; // initialization of the stream-timer structures
	__int64 end_time;
	LARGE_INTEGER end_time2;
	HANDLE tm1 = CreateWaitableTimer(NULL, false, NULL); // create timer
	end_time = -1 * stop * 10000000;
	end_time2.LowPart = (DWORD)(end_time & 0xFFFFFFFF);
	end_time2.HighPart = (LONG)(end_time >> 32);
	SetWaitableTimer(tm1, &end_time2, 0, NULL, NULL, false); // start the timer
	for (int i = 0; i < 7; i++) {
		params* param = (params*)malloc(sizeof(params));
		param->num = i;
		param->runflg = &runFlag;
		HANDLE t1 = CreateThread(NULL, 0, Thread1, param, 0, &targetThreadId); // generation of a thread and
		SetThreadPriority(t1, priority[i]);         // assigning priority to it
		PBOOL ptr1 = (PBOOL)malloc(sizeof(BOOL));
		GetThreadPriorityBoost(t1, ptr1);
		SetThreadPriorityBoost(t1, true); // checking the dynamic allocation of priorities
		CloseHandle(t1); // clean memory
	}
	WaitForSingleObject(tm1, INFINITE); // waiting the thread of timer
	runFlag = false; // ending working flag
	CloseHandle(tm1);
	cout << endl;
	for (int i = 0; i < 7; i++) {
		cout << i << " - " << counters[i] << endl; // print results
	}
	system("pause");
	return 0;
}

DWORD WINAPI Thread1(LPVOID prm) {
	while (1) {
		DWORD WINAPI thrdid = GetCurrentThreadId(); // the value of the identifier of the calling thread
		HANDLE WINAPI handle = OpenThread(THREAD_QUERY_INFORMATION, false, thrdid); //descriptor of the thread
		int WINAPI prio = GetThreadPriority(handle); // priority for the defined thread
		params arg = *((params*)prm);
		counters[arg.num]++;
		if (prio != priority[arg.num]) // displays when dynamic priority allocation is enabled
			cout << endl << "Priority of " << arg.num << " is " << priority[arg.num] << " " << prio << " changed" << endl;
		Sleep(0);
		if (*(arg.runflg) == false)
			break;
	}
	return 1;
}
