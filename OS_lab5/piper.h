#include <windows.h>
#include <iostream>

using namespace std;

static bool createAndConnectPipe(HANDLE& handle, LPCWSTR lpswPipeName) {
    char c;
    handle = CreateNamedPipe(
        lpswPipeName,      // имя канала
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
    if (!ConnectNamedPipe(

            handle,        // дескриптор канала
            (LPOVERLAPPED)NULL // связь синхронная
            ))
    {
        cerr << "The connection failed." << endl
             << "The last error code: " << GetLastError() << endl;
        CloseHandle(handle);
        cout << "Press any char to finish the server: ";
        cin >> c;
        return false;
    }

    return true;
}

static createFile (HANDLE& handle, LPCWSTR lpswPipeName)