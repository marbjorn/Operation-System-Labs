#include <windows.h>
#include <iostream>
#include "piper.h"

using namespace std;

int main()
{
    char c; // служебный символ
    HANDLE hNamedPipe;
    char lpszInMessage[80];                                       // для сообщения от клиента
    DWORD dwBytesRead;                                            // для количества прочитанных байтов
    char lpszOutMessage[] = "The server has received a message."; // обратное сообщение
    DWORD dwBytesWrite;                                           // для количества записанных байтов
    // создаем именованный канал для чтения
    createAndConnectPipe(hNamedPipe, "\\\\.\\pipe\\demo_pipe");
    
    // читаем сообщение от клиента
    if (!ReadFile(

            hNamedPipe,            // дескриптор канала
            lpszInMessage,         // адрес буфера для ввода данных
            sizeof(lpszInMessage), // число читаемых байтов
            &dwBytesRead,          // число прочитанных байтов
            (LPOVERLAPPED)NULL     // передача данных синхронная
            ))
    {
        cerr << "Data reading from the named pipe failed." << endl

             << "The last error code: " << GetLastError() << endl;
        CloseHandle(hNamedPipe);
        cout << "Press any char to finish the server: ";
        cin >> c;
        return 0;
    }
    // выводим полученное от клиента сообщение на консоль
    cout << "The server has received the following message from a client: "
         << endl
         << "\t" << lpszInMessage << endl;
    // отвечаем клиенту
    if (!WriteFile(

            hNamedPipe,             // дескриптор канала
            lpszOutMessage,         // адрес буфера для вывода данных
            sizeof(lpszOutMessage), // число записываемых байтов
            &dwBytesWrite,          // число записанных байтов
            (LPOVERLAPPED)NULL      // передача данных синхронная
            ))
    {
        cerr << "Data writing to the named pipe failed." << endl
             << "The last error code: " << GetLastError() << endl;
        CloseHandle(hNamedPipe);
        cout << "Press any char to finish the server: ";
        cin >> c;
        return 0;
    }
    // выводим посланное клиенту сообщение на консоль
    cout << "The server send the following message to a client: "
         << endl
         << "\t" << lpszOutMessage << endl;
    // закрываем дескриптор канала
    CloseHandle(hNamedPipe);
    // завершаем процесс
    cout << "Press any char to finish the server: ";
    cin >> c;
    return 0;
}