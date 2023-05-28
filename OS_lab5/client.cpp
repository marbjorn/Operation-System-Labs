#include <windows.h>
#include <iostream>
#include "piper.h"
#include "employee.h"



using namespace std;
int main()
{
    char c; // служебный символ
    HANDLE hNamedPipe;

    // связываемся с именованным каналом
    createFile(hNamedPipe, "\\\\.\\pipe\\demo_pipe");

    Sleep(5000);
    // закрываем дескриптор канала
    CloseHandle(hNamedPipe);
    return 0;
}