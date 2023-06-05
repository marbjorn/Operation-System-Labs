#include <iostream>
#include <windows.h>
#include "ThreadInfo.h"
#include <ranges>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <cstdint>

namespace rng = std::ranges;
namespace view = rng::views;

std::shared_ptr<HANDLE[]> fromMain;
std::shared_ptr<HANDLE[]> fromThread;
std::shared_ptr<int[]> array;
int arrSize;
CRITICAL_SECTION cs;

void printArray()
{
    for (int i : view::iota(0, arrSize))
    {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

DWORD WINAPI Marker(LPVOID v)
{
    std::shared_ptr<ThreadInfo> cur(static_cast<ThreadInfo*>(v)); // current ThreadInfo object

    EnterCriticalSection(&cs);

    auto g = boost::random::mt19937{static_cast<std::uint32_t>(cur->markerNum)};
    auto u = boost::random::uniform_int_distribution<>();

    std::shared_ptr<bool[]> marked(new bool[arrSize]); // array to track marked elements

    for (int i : view::iota(0, arrSize))
    {
        marked[i] = false;
    }

    LeaveCriticalSection(&cs);

    while (true)
    {
        int num = u(g);

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
        else
        { // if the element isn't equal 0
            // print info about thread
            std::cout << "\nMarker N." << cur->markerNum << "\nCan't modify the element with index: "
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

    SetEvent(fromThread[cur->markerNum - 1]);

    EnterCriticalSection(&cs);
    std::cout << "Thread N." << cur->markerNum << " is finished\n";
    LeaveCriticalSection(&cs);
    return 0;
}

int main()
{

    // initialization array
    std::cout << "Array size: " << std::endl;
    std::cin >> arrSize;

    array.reset(new int[arrSize]);
    for (int i = 0; i < arrSize; i++)
    {
        array[i] = 0;
    }

    // initialization Marker array
    std::cout << "Thread amount: " << std::endl;
    int tnum;
    std::cin >> tnum;
    std::shared_ptr<ThreadInfo*[]> tarr(new ThreadInfo*[tnum]); //ThreadInfo **tarr = new ThreadInfo *[tnum];
    DWORD dword;
    InitializeCriticalSection(&cs);

    // initization of handle arrays
    fromMain.reset(new HANDLE[tnum]);
    fromThread.reset(new HANDLE[tnum]);
    for (int i = 0; i < tnum; i++)
    {
        tarr.get()[i] = new ThreadInfo;
        fromMain.get()[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        fromThread.get()[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        tarr.get()[i]->markerNum = i + 1;
        tarr.get()[i]->handle = CreateThread(NULL, 0, Marker, static_cast<LPVOID>(tarr.get()[i]), 0, &dword);
        if (tarr.get()[i]->handle == NULL)
        {
            std::cerr << "Handle of Marker N." << i << " is NULL";
            return GetLastError();
        }
    }

    int n;
    for (int i : view::iota(0, tnum))
    {

        // waiting 'till all threads stop
        if (WaitForMultipleObjects(tnum, fromThread.get(), TRUE, INFINITE) == WAIT_FAILED)
        {
            std::cout << "Wait for multiple objects failed." << std::endl;
            std::cout << "Press any key to exit." << std::endl;
            system("pause");
        }

        // resume threads execution
        for (int j : view::iota(0, tnum))
            SetEvent(fromMain[j]);

        // input thread number to finish
        EnterCriticalSection(&cs);
        printArray();
        do
        {
            std::cout << "Enter the N. of thread to stop: ";
            std::cin >> n;
            if (tarr.get()[n - 1]->isStopped)
            {
                std::cout << "This thread is already finished\n";
            }
        } while (tarr.get()[n - 1]->isStopped);

        tarr.get()[n - 1]->isStopped = true;
        LeaveCriticalSection(&cs);

        // signal to finish current thread
        SetEvent(fromMain.get()[n - 1]);
        WaitForSingleObject(fromThread.get()[n - 1], INFINITE);

        // resume threads execution
        for (int j : view::iota(0, tnum))
        {
            SetEvent(fromMain.get()[j]);
            ResetEvent(fromMain.get()[j]);
        }
    }

    std::cout << "All threads are finished";
    // closing handles
    for (int i : view::iota(0, tnum))
    {
        delete tarr.get()[i];
        CloseHandle(tarr.get()[i]->handle);
        CloseHandle(fromThread.get()[i]);
        CloseHandle(fromMain.get()[i]);
    }

    DeleteCriticalSection(&cs);
    return 0;
}