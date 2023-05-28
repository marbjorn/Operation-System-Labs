#include <windows.h>
#include <iostream>
#include "piper.h"
#include "employee.h"
#include <fstream>

using namespace std;

fstream file;

// ������ ����������
void write(employee st, DWORD pos)
{
	file.seekp(sizeof(int) + pos * sizeof(st), ios::beg);
	file.write((char *)&st, sizeof(st));
}

// ������ ����������
void read(employee &st, DWORD pos)
{
	file.seekp(sizeof(int) + pos * sizeof(st), ios::beg);
	file.read((char *)&st, sizeof(st));
}

void init()
{
	cout << "Enter filename: ";
	char filename[256];
	cin >> filename;
	file.open(filename, ios::binary | ios::out | ios::in | ios::trunc);
	cout << "Enter record count: ";
	int N;
	cin >> N;
	HANDLE hWriteAccess = CreateEventA(0, TRUE, TRUE, "writeAccess");
	HANDLE hReadAccess = CreateEventA(0, TRUE, TRUE, "readAccess");
	file.write((char *)&N, sizeof(int));
	for (int i = 0; i < N; ++i)
	{
		employee st;
		cout << "\t\n___Record number " << i + 1 << " ____\n";
		cout << "Enter employee's number: ";
		cin >> st.num;
		cout << "Enter name: ";
		cin.get();
		cin.getline(st.name, 9);
		cout << "Enter hours: ";
		cin >> st.hours;
		file.write((char *)&st, sizeof(st));
	}
}

int main()
{
	init();
	char c; // ��������� ������
	HANDLE hNamedPipe;
	DWORD dwBytesRead;	// ��� ���������� ����������� ������
	DWORD dwBytesWrite; // ��� ���������� ���������� ������
	// ������� ����������� ����� ��� ������
	createAndConnectPipe(hNamedPipe, "\\\\.\\pipe\\demo_pipe");

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	// ��������� �������� ��������� STARTUPINFO �� ���������
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(NULL, "client.exe", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		cout << "The mew process is not created." << endl
			 << "Check a name of the process." << endl;
		return 0;
	}
	// ������ ��������� �� �������
	// ��������� ���������� ������
	Sleep(7000);

	CloseHandle(hNamedPipe);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	// ��������� �������
	cout << "Press any char to finish the server: ";
	cin >> c;
	return 0;
}