#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "message.h"
#include <vector>

HANDLE StartProcess(string command_line)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	if (!CreateProcess(NULL, const_cast<LPSTR>(command_line.c_str()), NULL, NULL,
					   FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		return NULL;
	}

	CloseHandle(pi.hThread);
	return pi.hProcess;
}

int main()
{
	std::string fileName;
	std::cout << "Enter file name:";
	std::cin >> fileName;
	std::fstream file(fileName, ios::binary | ios::out);
	int numberOfRecords;
	std::cout << "Enter number of records:";
	std::cin >> numberOfRecords;
	if (!file.is_open())
	{
		cout << "Error while creating file!";
		return 0;
	}
	file.close();

	int numberOfSenders;
	std::cout << "Enter number of Senders:";
	std::cin >> numberOfSenders;

	HANDLE *senders = new HANDLE[numberOfSenders];
	HANDLE *events = new HANDLE[numberOfSenders];

	HANDLE mutex = CreateMutex(NULL, FALSE, "mutex");
	HANDLE writeSem = CreateSemaphore(NULL, numberOfRecords, numberOfRecords, "write_sem");
	HANDLE readSem = CreateSemaphore(NULL, 0, numberOfRecords, "read_sem");
	if (!mutex || !writeSem || !readSem)
	{
		std::cout << "Error while creating syncronization objects";
		return -1;
	}

	// open processes
	for (int i = 0; i < numberOfSenders; ++i)
	{
		std::string commandLine = "sender.exe " + fileName + " " + to_string(numberOfRecords) + " " + to_string(i);

		HANDLE event = CreateEvent(NULL, FALSE, FALSE, (to_string(i) + "ready").c_str());
		events[i] = event;
		senders[i] = StartProcess(commandLine);
		if (senders[i] == NULL)
		{
			std::cout << "Error while creating process";
			return -1;
		}
	}

	WaitForMultipleObjects(numberOfSenders, events, TRUE, INFINITE);

	int action = 1;

	while (true)
	{
		std::cout << "1 to read message"
				  << "\n";
		std::cout << "0 to exit"
				  << "\n";
		std::cin >> action;
		if (action != 0 && action != 1)
		{
			std::cout << "Unknown command";
			continue;
		}
		if (action == 0)
		{
			break;
		}
		WaitForSingleObject(readSem, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		// read message
		file.open(fileName, ios::binary | ios::in);
		message mes;
		file >> mes;
		std::cout << "new message:" << mes.get_text() << "\n";
		std::vector<message> fileText;
		while (file >> mes)
		{
			fileText.push_back(mes);
		}
		file.close();
		file.open(fileName, ios::binary | ios::out);
		for (int i = 0; i < fileText.size(); ++i)
		{
			file << fileText[i];
		}
		file.close();
		ReleaseMutex(mutex);
		ReleaseSemaphore(writeSem, 1, NULL);
	}

	for (int i = 0; i < numberOfSenders; ++i)
	{
		CloseHandle(senders[i]);
		CloseHandle(events[i]);
	}

	CloseHandle(readSem);
	CloseHandle(writeSem);
	CloseHandle(mutex);
	delete[] senders;
	delete[] events;
	return 0;
}