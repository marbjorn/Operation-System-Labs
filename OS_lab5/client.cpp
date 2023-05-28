#include <windows.h>
#include <iostream>
#include "piper.h"
#include "employee.h"

HANDLE hModify;
HANDLE hRead;
HANDLE hReady;

void init() {

}
using namespace std;
int main()
{
    char c; // служебный символ
    HANDLE hNamedPipe;
    hModify = OpenEventA(EVENT_MODIFY_STATE, FALSE, "Modify");
    hRead = OpenEventA(EVENT_MODIFY_STATE, FALSE, "Read");
    hReady = OpenEventA(EVENT_MODIFY_STATE, FALSE, "Ready");
    /*
    // связываемся с именованным каналом
    createFile(hNamedPipe, "\\\\.\\pipe\\demo_pipe");
    */
    SetEvent(hReady);
    int command;
    do
    {
        cout << "1: modify" << endl
             << "2: read" << endl
             << "3: exit" << endl;
        cin >> command;
        switch (command)
        {
        case 1:
        {
            // modify
            cout << "modify" << endl;
            SetEvent(hModify);
            break;
        }
        case 2:
        {
            // read
            cout << "read" << endl;
            SetEvent(hRead);
            break;
        }
        default:
        {
            if (command == 3)
            {
                cout << "exit" << endl;
            }
            else
            {
                cout << "Unknown command, try again" << endl;
            }
        }
        }
    } while (command != 3);
    cout << "out of cycle" << endl;
    Sleep(5000);
    // закрываем дескриптор канала
    CloseHandle(hNamedPipe);
    CloseHandle(hModify);
    CloseHandle(hRead);
    CloseHandle(hReady);
    return 0;
}