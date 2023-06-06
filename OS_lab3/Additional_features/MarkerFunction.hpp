#include <ranges>
#include <string>

namespace rng = std::ranges;
namespace view = rng::views;

std::string printArray(const int* array, const int& arrSize)
{
    std::string str = "";
    
    for (int i : view::iota(0, arrSize))
    {
        str += std::to_string(array[i]);
        if (i < arrSize - 1) {
            str = str + " ";
        }
    }
    return str;
}

//check if the number is natural
bool isNaturalNumber(const int number)  {
    return number > 0;
}

//remove marks of current value
void removeMarksFromElements(int* array, const int& arrSize, const int& numToRemove) {
    for (int i : view::iota(0, arrSize)) {
        if (array[i] == numToRemove) {
            array[i] = 0;
        }
    }
}