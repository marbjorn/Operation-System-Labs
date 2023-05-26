#include <windows.h>
#include <iostream>

using namespace std;

int main()
{
    char c; // служебный символ
    HANDLE hNamedPipe;
    char lpszInMessage[80];                                       // дл€ сообщени€ от клиента
    DWORD dwBytesRead;                                            // дл€ количества прочитанных байтов
    char lpszOutMessage[] = "The server has received a message."; // обратное сообщение
    DWORD dwBytesWrite;                                           // дл€ количества записанных байтов
    // создаем именованный канал дл€ чтени€
    hNamedPipe = CreateNamedPipe(
        "\\\\.\\pipe\\demo_pipe",      // им€ канала
        PIPE_ACCESS_DUPLEX,            // читаем из канала и пишем в канал
        PIPE_TYPE_MESSAGE | PIPE_WAIT, // синхронна€ передача сообщений
        1,                             // максимальное количество экземпл€ров канала
        0,                             // размер выходного буфера по умолчанию
        0,                             // размер входного буфера по умолчанию
        INFINITE,                      // клиент ждет св€зь 500 мс
        (LPSECURITY_ATTRIBUTES)NULL    // защита по умолчанию
    );
    // провер€ем на успешное создание
    if (hNamedPipe == INVALID_HANDLE_VALUE)
    {
        cerr << "Creation of the named pipe failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any char to finish server: ";
        cin >> c;
        return 0;
    }
    // ждем, пока клиент св€жетс€ с каналом
    cout << "The server is waiting for connection with a client." << endl;
    if (!ConnectNamedPipe(

            hNamedPipe,        // дескриптор канала
            (LPOVERLAPPED)NULL // св€зь синхронна€
            ))
    {
        cerr << "The connection failed." << endl
             << "The last error code: " << GetLastError() << endl;
        CloseHandle(hNamedPipe);
        cout << "Press any char to finish the server: ";
        cin >> c;
        return 0;
    }
    // читаем сообщение от клиента
    if (!ReadFile(

            hNamedPipe,            // дескриптор канала
            lpszInMessage,         // адрес буфера дл€ ввода данных
            sizeof(lpszInMessage), // число читаемых байтов
            &dwBytesRead,          // число прочитанных байтов
            (LPOVERLAPPED)NULL     // передача данных синхронна€
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
            lpszOutMessage,         // адрес буфера дл€ вывода данных
            sizeof(lpszOutMessage), // число записываемых байтов
            &dwBytesWrite,          // число записанных байтов
            (LPOVERLAPPED)NULL      // передача данных синхронна€
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