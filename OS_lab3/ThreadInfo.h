#include <windows.h>

struct ThreadInfo
{
    // handle
    HANDLE handle;

    // Marker number
    int markerNum;

    // stopped
    bool isStopped = false;

    int markedElementsNum = 0;
};