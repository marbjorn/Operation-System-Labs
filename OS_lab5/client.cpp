#include <windows.h>
#include <iostream>

using namespace std;
int main()
{
    char c; // служебный символ
    HANDLE hNamedPipe;
    char machineName[80];
    char pipeName[80];
    char lpszOutMessage[] = "How do you do server?"; // сообщение серверу
    DWORD dwBytesWritten;                            // для числа записанных байтов
    char lpszInMessage[80];                          // для сообщения от сервера
    DWORD dwBytesRead;                               // для числа прочитанных байтов
    // вводим имя машины в сети, на которой работает сервер

    cout << "Enter a name of the server machine: ";
    cin >> machineName;
    // подставляем имя машины в имя канала
    wsprintf(pipeName, "\\\\%s\\pipe\\demo_pipe",
             machineName);
    // связываемся с именованным каналом
    hNamedPipe = CreateFile(
        pipeName,                           // имя канала
        GENERIC_READ | GENERIC_WRITE,       // читаем и записываем в канал
        FILE_SHARE_READ | FILE_SHARE_WRITE, // разрешаем чтение и запись в канал
        (LPSECURITY_ATTRIBUTES)NULL,        // защита по умолчанию
        OPEN_EXISTING,                      // открываем существующий канал
        FILE_ATTRIBUTE_NORMAL,              // атрибуты по умолчанию
        (HANDLE)NULL                        // дополнительных атрибутов нет
    );
    // проверяем связь с каналом
    if (hNamedPipe == INVALID_HANDLE_VALUE)
    {
        cerr << "Connection with the named pipe failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any char to finish the client: ";
        cin >> c;
        return 0;
    }
    // пишем в именованный канал
    if (!WriteFile(
            hNamedPipe,             // дескриптор канала
            lpszOutMessage,         // данные
            sizeof(lpszOutMessage), // размер данных
            &dwBytesWritten,        // количество записанных байтов
            (LPOVERLAPPED)NULL      // синхронная запись
            ))
    {

        // ошибка записи
        cerr << "Writing to the named pipe failed: " << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any char to finish the client: ";
        cin >> c;
        CloseHandle(hNamedPipe);
        return 0;
    }
    // выводим посланное сообщение на консоль
    cout << "The client has send the following message to a server: "
         << endl
         << "\t" << lpszOutMessage << endl;
    // читаем из именованного канала
    if (!ReadFile(
            hNamedPipe,            // дескриптор канала
            lpszInMessage,         // данные
            sizeof(lpszInMessage), // размер данных
            &dwBytesRead,          // количество записанных байт
            (LPOVERLAPPED)NULL     // синхронная запись
            ))
    {

        // ошибка записи
        cerr << "Reading to the named pipe failed: " << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any char to finish the client: ";
        cin >> c;

        CloseHandle(hNamedPipe);
        return 0;
    }
    // выводим полученное сообщение на консоль
    cout << "The client has received the following message from a server: "
         << endl
         << "\t" << lpszInMessage << endl;
    // закрываем дескриптор канала
    CloseHandle(hNamedPipe);
    // завершаем процесс
    cout << "Press any char to finish the client: ";
    cin >> c;
    return 0;
}