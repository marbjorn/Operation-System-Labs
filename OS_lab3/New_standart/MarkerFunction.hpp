#include <iostream>
#include <ranges>
#include <string>
#include <boost/lexical_cast.hpp>

namespace rng = std::ranges;
namespace view = rng::views;

std::string printArray(const int* array, const int& arrSize)
{
    std::string str = "";
    
    for (int i : view::iota(0, arrSize))
    {
        str += boost::lexical_cast<std::string>(array[i]);
        if (i < arrSize - 1) {
            str = str + " ";
        }
    }
    return str;
}

