#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <iostream>
using namespace std;
#define DEF_THREADS 7
#define DEF_TTL 10

DWORD WINAPI threadHandler(LPVOID);
HANDLE initTimer(int sec);
int getPriorityIndex(DWORD prClass);
int isFinish = 0;
long counters[7] = { 0, 0, 0, 0, 0, 0, 0 };
int priorities[7] = { THREAD_PRIORITY_IDLE, THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL,
THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST, THREAD_PRIORITY_TIME_CRITICAL };
char charPrio[7][10] = { "IDLE", "LOWEST", "BELOW", "NORMAL", "ABOVE", "HIGHEST", "TIME_CRIT" };
char charProcPrio[6][10] = { "IDLE", "BELOW", "NORMAL", "ABOVE", "HIGH", "REAL-TIME" };
int procPriorities[6] = { IDLE_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS,
ABOVE_NORMAL_PRIORITY_CLASS, HIGH_PRIORITY_CLASS, REALTIME_PRIORITY_CLASS };
int priorityBoost[7] = { 0, 0, 0, 0, 0, 0, 0 };
int priorityChange[7] = { 0, 0, 0, 0, 0, 0, 0 };

int main(int argc, char* argv[]) {
	int numThreads = DEF_THREADS;
	int threadLive = DEF_TTL;
	if (argc < 2)
		cout << "Using default numThreads = " << numThreads << " and default time to live = " << threadLive << endl;
	else if (argc < 3)
		cout << "Using default time to live = " << threadLive << endl;
	else {
		numThreads = atoi(argv[1]);
		threadLive = atoi(argv[2]);
		if (numThreads <= 0 || threadLive <= 0) {
			cout << "All arguments must be numbers!!!!" << endl;
			exit(0);
		}
	}
	HANDLE t = initTimer(threadLive);
	HANDLE t1;
	// set the priority of the process = IDLE (or other)
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	for (int i = 0; i < numThreads; i++) {
		t1 = CreateThread(NULL, 0, threadHandler, (LPVOID)i, 0, NULL);
		SetThreadPriority(t1, priorities[i]);
		SetThreadPriorityBoost(t1, true);
		GetThreadPriorityBoost(t1, &priorityBoost[i]);
		CloseHandle(t1);
	}
	WaitForSingleObject(t, INFINITE); // Wait all threads - comment-uncommitted line depending on the task
	CloseHandle(t);
	isFinish = 1;
	char hasBoost[4];
	char wasChanged[4];
	int priorIdx = getPriorityIndex(GetPriorityClass(GetCurrentProcess()));
	cout << "Result of work:" << endl;
	cout << "Process priority: " << charProcPrio[priorIdx] << endl;
	cout << "Priority\tHas Boost\tWas changed\tCounter" << endl;
	for (int i = 0; i < 7; i++) {
		priorityBoost[i] == 0 ? strcpy(hasBoost, "NO") : strcpy(hasBoost, "YES");
		priorityChange[i] == 0 ? strcpy(wasChanged, "NO") : strcpy(wasChanged, "YES");
		printf("%8s\t%9s\t%10s\t%7d\n", charPrio[i], hasBoost, wasChanged, counters[i]);
	}
	system("pause");
	return 0;
}
DWORD WINAPI threadHandler(LPVOID prm) {
	int myNum = (int)prm;
	int priority = 0;
	for (;; ) {
		++counters[myNum];
		priority = GetThreadPriority(GetCurrentThread());
		if (priority != priorities[myNum])
			priorityChange[myNum] = 1;
		if (isFinish)
			break;
		Sleep(0);
	}
	return 0;
}
HANDLE initTimer(int sec) {
	__int64 end_time;
	LARGE_INTEGER end_time2;
	HANDLE tm = CreateWaitableTimer(NULL, false, "timer");
	end_time = -1 * sec * 1000000;
	end_time2.LowPart = (DWORD)(end_time & 0xFFFFFFFF);
	end_time2.HighPart = (LONG)(end_time >> 32);
	SetWaitableTimer(tm, &end_time2, 0, NULL, NULL, false);
	return tm;
}
int getPriorityIndex(DWORD prClass) {
	for (int i = 0; i < 6; ++i) {
		if (procPriorities[i] == prClass)
			return i;
	}
	return 0;
}
