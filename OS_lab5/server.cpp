#include <windows.h>
#include <iostream>
#include "piper.h"

using namespace std;

int main()
{
    char c; // ��������� ������
    HANDLE hNamedPipe;
    char lpszInMessage[80];                                       // ��� ��������� �� �������
    DWORD dwBytesRead;                                            // ��� ���������� ����������� ������
    char lpszOutMessage[] = "The server has received a message."; // �������� ���������
    DWORD dwBytesWrite;                                           // ��� ���������� ���������� ������
    // ������� ����������� ����� ��� ������
    createAndConnectPipe(hNamedPipe, "\\\\.\\pipe\\demo_pipe");
    
    // ������ ��������� �� �������
    if (!ReadFile(

            hNamedPipe,            // ���������� ������
            lpszInMessage,         // ����� ������ ��� ����� ������
            sizeof(lpszInMessage), // ����� �������� ������
            &dwBytesRead,          // ����� ����������� ������
            (LPOVERLAPPED)NULL     // �������� ������ ����������
            ))
    {
        cerr << "Data reading from the named pipe failed." << endl

             << "The last error code: " << GetLastError() << endl;
        CloseHandle(hNamedPipe);
        cout << "Press any char to finish the server: ";
        cin >> c;
        return 0;
    }
    // ������� ���������� �� ������� ��������� �� �������
    cout << "The server has received the following message from a client: "
         << endl
         << "\t" << lpszInMessage << endl;
    // �������� �������
    if (!WriteFile(

            hNamedPipe,             // ���������� ������
            lpszOutMessage,         // ����� ������ ��� ������ ������
            sizeof(lpszOutMessage), // ����� ������������ ������
            &dwBytesWrite,          // ����� ���������� ������
            (LPOVERLAPPED)NULL      // �������� ������ ����������
            ))
    {
        cerr << "Data writing to the named pipe failed." << endl
             << "The last error code: " << GetLastError() << endl;
        CloseHandle(hNamedPipe);
        cout << "Press any char to finish the server: ";
        cin >> c;
        return 0;
    }
    // ������� ��������� ������� ��������� �� �������
    cout << "The server send the following message to a client: "
         << endl
         << "\t" << lpszOutMessage << endl;
    // ��������� ���������� ������
    CloseHandle(hNamedPipe);
    // ��������� �������
    cout << "Press any char to finish the server: ";
    cin >> c;
    return 0;
}