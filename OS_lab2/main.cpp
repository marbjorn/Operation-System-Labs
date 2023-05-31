#include <iostream>
#include <windows.h>
#include "ArrData.h"
#include <math.h>

CRITICAL_SECTION cs;
int min;
int max;
double average;

bool createThread(DWORD WINAPI func(LPVOID v), ArrData* array, DWORD& dword) {
    HANDLE hThread = CreateThread(  //create thread
        NULL,                       //define if this thread can be inherited
        0,                          //stack size
        func,                       //function name
        (void*)array,               //argument to pass
        0,                          //special flag to postpone the start of thread, etc. 
        &dword                      //variable to save thread identificator
    );

    if (hThread == NULL) {
        std::cerr << "ERROR: Starting of thread is failed" << std::endl;
        return false;
    }

    if (WaitForSingleObject(hThread, INFINITE)) {  //waiting for thread 
        std::cerr << "ERROR: Failed waiting for the thread" << std::endl;
        return false;
    }
    CloseHandle(hThread);
    return true;
}

void printArray(const ArrData* array) {
    std::cout << "Array: ";
    for (int i = 0; i < array->N; i++) {
        std::cout << array->arr[i] <<" ";
    }
    std::cout << "\n";
}

//function to find min and max elements of array
DWORD WINAPI MinMax(LPVOID v) {
    
    ArrData* array = (ArrData*) v;
    max = INT_MIN;
    min = INT_MAX;
    for (int i = 0; i < array->N; i++) {
        EnterCriticalSection(&cs);
        if (min > array->arr[i]) {
            min = array->arr[i];
        }
        if (max < array->arr[i]) {
            max = array->arr[i];
        }
        LeaveCriticalSection(&cs);
        Sleep(7);
    }
    EnterCriticalSection(&cs);
    std::cout << "Max is " << max << ", min is " << min << std::endl;
    LeaveCriticalSection(&cs);
    return 0;
}

//function to find average value of elements in array
DWORD WINAPI Average(LPVOID v) {
    ArrData* array = (ArrData*) v;
    average = 0;
    for (int i = 0; i < array->N; i++) {
        EnterCriticalSection(&cs);
        average += (double)array->arr[i];
        LeaveCriticalSection(&cs);
        Sleep(12);
        
    }
    EnterCriticalSection(&cs);
    average /= (double)array->N;
    std::cout << "Average is " << average << std::endl;
    LeaveCriticalSection(&cs);
    return 0;
}

int N;

int main() {
    std::cout << "Enter N: ";
    std::cin >> N;
    ArrData* array = new ArrData(N); //initialisation of structure instance
    DWORD dwThread;

    //filling the array 
    for (int i = 0; i < N; i++) {
        std::cout << "Enter the " << i+1 << " element of array: ";
        std::cin >> array->arr[i];
    }

    printArray(array);

    InitializeCriticalSection(&cs); //initialization of critical section

    createThread(MinMax, array, dwThread);
    createThread(Average, array, dwThread);

    DeleteCriticalSection(&cs); //deleting critical section

    //changing elements with min and max values to average value
    for (int i = 0; i < N; i++) {
        if (array->arr[i] == min || array->arr[i] == max)  {
            array->arr[i] = (int)round(average);
        }
    }

    std::cout << "(modified) ";
    printArray(array);
    delete array;
    return 0;
}