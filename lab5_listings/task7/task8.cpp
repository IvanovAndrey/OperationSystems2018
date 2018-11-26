#include <stdio.h>
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

void thread() {
	while (true) {
		Sleep(2000);
	}
}

int main(int argc, char *argv[]) {
	BOOL dynamic;
	HANDLE processHandle, mainThread, secondThread;     // threads
	DWORD secondID; processHandle = GetCurrentProcess();// descriptor of the current process
	mainThread = GetCurrentThread();    // descriptor of the current thread
	//create a second thread
	secondThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, NULL, NULL, &secondID); // create thread 2
	//print default values
	cout << "0 - dynamic enable, 1-disabled." << endl;
	GetProcessPriorityBoost(processHandle, &dynamic); // value of dynamic process priority
	cout << "Process dynamically default is " << dynamic << endl;
	GetThreadPriorityBoost(mainThread, &dynamic);
	cout << "Main thread dynamic default is " << dynamic << endl;
	GetThreadPriorityBoost(secondThread, &dynamic);
	cout << "Second thread dynamic default is " << dynamic << endl << endl;
	cout << "Change Second thread priority" << endl;
	if (!SetThreadPriorityBoost(secondThread, true)) { // increase priority of thread 2
		cout << "Error on thread change!" << endl;
		return 2;
	}
	GetProcessPriorityBoost(processHandle, &dynamic);
	cout << "Process dynamically default is " << dynamic << endl;
	GetThreadPriorityBoost(mainThread, &dynamic);
	cout << "Main thread dynamic default is " << dynamic << endl;
	GetThreadPriorityBoost(secondThread, &dynamic);
	cout << "Second thread dynamic default is " << dynamic << endl << endl;
	//change process dynamically
	if (!SetProcessPriorityBoost(processHandle, true)) { // increase priority of process
		cout << "Error on prir change!" << endl;
		return 1;
	}
	cout << "After process dynamic drop:" << endl;
	GetProcessPriorityBoost(processHandle, &dynamic);
	cout << "Process is " << dynamic << endl;
	GetThreadPriorityBoost(mainThread, &dynamic);
	cout << "Main thread is " << dynamic << endl;
	GetThreadPriorityBoost(secondThread, &dynamic);
	cout << "Second thread is " << dynamic << endl << endl;
	//may be can change thread dynamic prio?
	if (!SetThreadPriorityBoost(secondThread, false)) { // lower priority of thread 2
		cout << "We cannot change if process ban dynamic prio!!!" << endl;
		return 3;
	}
	else {
		cout << "Thread dynamic prio changed, but process bans it!!!" << endl;
		GetProcessPriorityBoost(processHandle, &dynamic);
		cout << "Process is " << dynamic << endl;
		GetThreadPriorityBoost(mainThread, &dynamic);
		cout << "Main thread is " << dynamic << endl;
		GetThreadPriorityBoost(secondThread, &dynamic);
		cout << "Second thread is " << dynamic << endl;
	}
	system("pause");
	return 0;
}
