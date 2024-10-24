#pragma once
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

    template<typename Ty>
    Ty max(Ty in_1, Ty in_2)
    {
        return in_1 > in_2 ? in_1 : in_2;
    }

    template<typename Ty>
    Ty min(Ty in_1, Ty in_2)
    {
        return in_1 > in_2 ? in_2 : in_1;
    }
}