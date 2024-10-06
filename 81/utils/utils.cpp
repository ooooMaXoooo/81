#include "utils.h"

#include <vector>
#include <iostream>

namespace utils
{
    template<typename T>
    void printVector(const std::vector<T>& vec)
    {
        for (const auto& elem : vec)
        {
            std::cout << elem << '\n';
        }
        std::cout << std::flush;
    }
}