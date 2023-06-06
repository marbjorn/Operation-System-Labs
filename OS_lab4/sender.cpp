#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "message.h"

int main(int argc, char *argv[])
{
	// open syncronization objects
	HANDLE ready = OpenEvent(EVENT_MODIFY_STATE, FALSE, (to_string(atoi(argv[3])) + "ready").c_str());
	HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "mutex");
	HANDLE writeSem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "write_sem");
	HANDLE readSem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "read_sem");
	if (!ready || !readSem || !writeSem || !mutex)
	{
		std::cout << "Error while opening objects"
				  << "\n";
		return -1;
	}
	SetEvent(ready);
	std::fstream file;
	int action;
	while (true)
	{
		std::cout << "1 to write message"
				  << "\n";
		std::cout << "0 to exit"
				  << "\n";
		std::cin >> action;
		if (action != 0 && action != 1)
		{
			std::cout << "Unknown command"
					  << "\n";
			continue;
		}
		if (action == 0)
		{
			break;
		}
		std::cin.ignore();
		string text;
		// sending message
		std::cout << "Enter message text:";
		getline(std::cin, text);
		WaitForSingleObject(writeSem, INFINITE);
		WaitForSingleObject(mutex, INFINITE);

		message mes(text);
		file.open(argv[1], ios::binary | ios::app);
		file << mes;
		file.close();
		ReleaseMutex(mutex);
		ReleaseSemaphore(readSem, 1, NULL);
		std::cout << "Written succesfully"
				  << "\n";
	}

	CloseHandle(readSem);
	CloseHandle(writeSem);
	CloseHandle(mutex);
	CloseHandle(ready);

	return 0;
}