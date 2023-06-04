#include <iostream>
#include <windows.h>
#include "ArrData.h"
#include <math.h>

int minValue;
int maxValue;
double averageValue;

bool createThread(DWORD WINAPI func(LPVOID v), ArrData *array, DWORD &dword)
{
    HANDLE hThread = CreateThread( // create thread
        NULL,                      // define if this thread can be inherited
        0,                         // stack size
        func,                      // function name
        (void *)array,             // argument to pass
        0,                         // special flag to postpone the start of thread, etc.
        &dword                     // variable to save thread identificator
    );

    // if thread is not started
    if (hThread == NULL)
    {
        std::cerr << "ERROR: Starting of thread is failed" << std::endl;
        return false;
    }

    if (WaitForSingleObject(hThread, INFINITE))
    { // waiting for thread
        std::cerr << "ERROR: Failed waiting for the thread" << std::endl;
        return false;
    }

    CloseHandle(hThread);
    return true;
}

void printArray(const ArrData *array)
{
    std::cout << "Array: ";
    for (int i = 0; i < array->N; i++)
    {
        std::cout << array->arr[i] << " ";
    }
    std::cout << "\n";
}

// function to find min and max elements of array
DWORD WINAPI MinMax(LPVOID v)
{

    ArrData *array = (ArrData *)v;
    maxValue = INT_MIN;
    minValue = INT_MAX;
    for (int i = 0; i < array->N; i++)
    {
        if (minValue > array->arr[i])
        {
            minValue = array->arr[i];
        }
        Sleep(7);
        if (maxValue < array->arr[i])
        {
            maxValue = array->arr[i];
        }
        Sleep(7);
    }

    std::cout << "Max is " << maxValue << ", min is " << minValue << std::endl;

    return 0;
}

// function to find average value of elements in array
DWORD WINAPI Average(LPVOID v)
{
    ArrData *array = (ArrData *)v;
    averageValue = 0;
    for (int i = 0; i < array->N; i++)
    {
        averageValue += (double)array->arr[i];
        Sleep(12);
    }

    averageValue /= (double)array->N;
    std::cout << "Average is " << averageValue << std::endl;

    return 0;
}

int N;

int main()
{
    std::cout << "Enter N: ";
    std::cin >> N;
    ArrData *array = new ArrData(N); // initialization of structure instance
    DWORD dwThread;

    // filling the array
    for (int i = 0; i < N; i++)
    {
        std::cout << "Enter the " << i + 1 << " element of array: ";
        std::cin >> array->arr[i];
    }

    printArray(array);

    createThread(MinMax, array, dwThread);
    createThread(Average, array, dwThread);

    // changing elements with min and max values to average value
    for (int i = 0; i < N; i++)
    {
        if (array->arr[i] == minValue || array->arr[i] == maxValue)
        {
            array->arr[i] = (int)round(averageValue);
        }
    }

    std::cout << "(modified) ";
    printArray(array);
    delete array;
    return 0;
}