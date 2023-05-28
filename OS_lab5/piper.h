#include <windows.h>
#include <iostream>
#include "employee.h"

using namespace std;
const int EMPLOYEELEN = sizeof(employee);

static bool createAndConnectPipe(HANDLE &handle, LPCSTR lpswPipeName)
{
    char c;
    handle = CreateNamedPipeA(
        lpswPipeName,                  // имя канала
        PIPE_ACCESS_DUPLEX,            // читаем из канала и пишем в канал
        PIPE_TYPE_MESSAGE | PIPE_WAIT, // синхронная передача сообщений
        1,                             // максимальное количество экземпляров канала
        0,                             // размер выходного буфера по умолчанию
        0,                             // размер входного буфера по умолчанию
        INFINITE,                      // клиент ждет связь 500 мс
        (LPSECURITY_ATTRIBUTES)NULL    // защита по умолчанию
    );
    // проверяем на успешное создание
    if (handle == INVALID_HANDLE_VALUE)
    {
        cerr << "Creation of the named pipe failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any char to finish server: ";
        cin >> c;
        return false;
    }
    // ждем, пока клиент свяжется с каналом
    cout << "The server is waiting for connection with a client." << endl;
    /*if (!ConnectNamedPipe(

            handle,            // дескриптор канала
            (LPOVERLAPPED)NULL // связь синхронная
            ))
    {
        cerr << "The connection failed." << endl
             << "The last error code: " << GetLastError() << endl;
        CloseHandle(handle);
        cout << "Press any char to finish the server: ";
        cin >> c;
        return false;
    }*/

    return true;
}

static bool createFile(HANDLE &handle, LPCSTR lpswPipeName)
{

    char c;
    handle = CreateFileA(
        lpswPipeName,                       // имя канала
        GENERIC_READ | GENERIC_WRITE,       // читаем и записываем в канал
        FILE_SHARE_READ | FILE_SHARE_WRITE, // разрешаем чтение и запись в канал
        (LPSECURITY_ATTRIBUTES)NULL,        // защита по умолчанию
        OPEN_EXISTING,                      // открываем существующий канал
        FILE_ATTRIBUTE_NORMAL,              // атрибуты по умолчанию
        (HANDLE)NULL                        // дополнительных атрибутов нет
    );

    // проверяем связь с каналом
    if (handle == INVALID_HANDLE_VALUE)
    {
        cerr << "Connection with the named pipe failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any char to finish the client: ";
        cin >> c;
        return false;
    }

    return true;
}

static bool readFile(HANDLE &handle, char (&lpszInMessage)[EMPLOYEELEN], DWORD &dwBytesRead)
{
    char c;
    if (!ReadFile(

            handle,                // дескриптор канала
            lpszInMessage,         // адрес буфера для ввода данных
            sizeof(lpszInMessage), // число читаемых байтов
            &dwBytesRead,          // число прочитанных байтов
            (LPOVERLAPPED)NULL     // передача данных синхронная
            ))
    {
        cerr << "Data reading from the named pipe failed." << endl

             << "The last error code: " << GetLastError() << endl;
        CloseHandle(handle);
        cout << "Press any char to finish the server: ";
        cin >> c;
        return 0;
    }
}

static bool writeFile(HANDLE &handle, char (&lpszOutMessage)[EMPLOYEELEN], DWORD &dwBytesWrite)
{
    char c;
    if (!WriteFile(

            handle,                 // дескриптор канала
            lpszOutMessage,         // адрес буфера для вывода данных
            sizeof(lpszOutMessage), // число записываемых байтов
            &dwBytesWrite,          // число записанных байтов
            (LPOVERLAPPED)NULL      // передача данных синхронная
            ))
    {
        cerr << "Data writing to the named pipe failed." << endl
             << "The last error code: " << GetLastError() << endl;
        CloseHandle(handle);
        cout << "Press any char to finish the server: ";
        cin >> c;
        return 0;
    }
}