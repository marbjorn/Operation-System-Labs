#include <iostream>
#include <windows.h>
#include "ThreadInfo.h"

HANDLE *fromMain;
HANDLE *fromThread;
int *array;
int arrSize;
CRITICAL_SECTION cs;

void printArray()
{
    for (int i = 0; i < arrSize; i++)
    {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

DWORD WINAPI Marker(LPVOID v)
{
    auto cur = static_cast<ThreadInfo *>(v); // current ThreadInfo object

    EnterCriticalSection(&cs);
    srand(cur->markerNum);
    bool *marked = new bool[arrSize]; // array to track marked elements
    for (int i = 0; i < arrSize; i++)
    {
        marked[i] = false;
    }
    LeaveCriticalSection(&cs);

    while (true)
    {
        int num = rand();
        EnterCriticalSection(&cs);
        // std::cout << "thr " << cur->num << " rand " << num << " ans " << num%arr_size << "\n";
        LeaveCriticalSection(&cs);
        num %= (arrSize); // random number

        EnterCriticalSection(&cs);
        if (array[num] == 0)
        { // if the element is equal 0
            Sleep(5);
            array[num] = cur->markerNum;
            marked[num] = true;
            ++cur->markedElementsNum;
            LeaveCriticalSection(&cs);
            Sleep(5);
        }
        else if (array[num] != 0)
        { // if the element isn't equal 0
            // print info about thread
            std::cout << "\nMarker №" << cur->markerNum << "\nCan't modify the element with index: "
                      << num << "\nNumber of marked elements: " << cur->markedElementsNum << std::endl
                      << std::endl;
            LeaveCriticalSection(&cs);

            SetEvent(fromThread[cur->markerNum - 1]);
            WaitForSingleObject(fromMain[cur->markerNum - 1], INFINITE);
            ResetEvent(fromThread[cur->markerNum - 1]);
            ResetEvent(fromMain[cur->markerNum - 1]);
            WaitForSingleObject(fromMain[cur->markerNum - 1], INFINITE);

            // got signal to finish thread
            if (cur->isStopped == true)
            {
                EnterCriticalSection(&cs);
                for (int i = 0; i < arrSize; i++)
                { // null all marked elements
                    if (marked[i])
                    {
                        array[i] = 0;
                    }
                }
                cur->markedElementsNum = 0;
                LeaveCriticalSection(&cs);
                break;
            }
        }
    }
    delete[] marked;

    SetEvent(fromThread[cur->markerNum - 1]);

    EnterCriticalSection(&cs);
    std::cout << "Thread №" << cur->markerNum << " is finished\n";
    LeaveCriticalSection(&cs);
    return 0;
}

int main()
{

    // initialization array
    std::cout << "Array size: " << std::endl;
    std::cin >> arrSize;

    array = new int[arrSize];
    for (int i = 0; i < arrSize; i++)
    {
        array[i] = 0;
    }

    // initialization Marker array
    std::cout << "Thread number: " << std::endl;
    int tnum;
    std::cin >> tnum;
    ThreadInfo **tarr = new ThreadInfo *[tnum];
    DWORD dword;
    InitializeCriticalSection(&cs);

    // initization of handle arrays
    fromMain = new HANDLE[tnum];
    fromThread = new HANDLE[tnum];
    for (int i = 0; i < tnum; i++)
    {
        tarr[i] = new ThreadInfo;
        fromMain[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        fromThread[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        (*tarr[i]).markerNum = i + 1;
        (*tarr[i]).handle = CreateThread(NULL, 0, Marker, static_cast<LPVOID>(tarr[i]), 0, &dword);
        if ((*tarr[i]).handle == NULL)
        {
            return GetLastError();
        }
    }

    int n;
    for (int i = 0; i < tnum; i++)
    {

        // waiting 'till all threads stop
        if (WaitForMultipleObjects(tnum, fromThread, TRUE, INFINITE) == WAIT_FAILED)
        {
            std::cout << "Wait for multiple objects failed." << std::endl;
            std::cout << "Press any key to exit." << std::endl;
            system("pause");
        }

        // resume threads execution
        for (int j = 0; j < tnum; j++)
            SetEvent(fromMain[j]);

        // input thread number to finish
        EnterCriticalSection(&cs);
        printArray();
        do
        {
            std::cout << "Enter the num of thread to stop: ";
            std::cin >> n;
            if (tarr[n - 1]->isStopped)
            {
                std::cout << "This thread is already finished\n";
            }
        } while (tarr[n - 1]->isStopped);

        (*tarr[n - 1]).isStopped = true;
        LeaveCriticalSection(&cs);

        // signal to finish current thread
        SetEvent(fromMain[n - 1]);
        WaitForSingleObject(fromThread[n - 1], INFINITE);

        // resume threads execution
        for (int j = 0; j < tnum; j++)
        {
            SetEvent(fromMain[j]);
            ResetEvent(fromMain[j]);
        }
    }

    std::cout << "All threads are finished";
    // closing handles
    for (int i = 0; i < tnum; i++)
    {
        delete tarr[i];
        CloseHandle(tarr[i]->handle);
        CloseHandle(fromThread[i]);
        CloseHandle(fromMain[i]);
    }

    DeleteCriticalSection(&cs);
    delete[] tarr;
    delete[] fromThread;
    delete[] fromMain;

    delete[] array;
    return 0;
}