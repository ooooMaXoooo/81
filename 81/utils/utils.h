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

    template<typename Ty>
    void maxAndSecondMax(Ty* arr, unsigned int size, Ty& max, Ty& s_max, unsigned int* b_indice, unsigned int* s_b_indice) {
        max = INT_MIN;
        s_max = INT_MIN;

        for (int i = 0; i < size; ++i) {
            if (arr[i] > max) {
                s_max = max;
                max = arr[i];

                if (s_b_indice)
                    *s_b_indice = *b_indice;

                if (b_indice)
                    *b_indice = i;
            }
            else if (arr[i] > s_max && arr[i] != max) {
                s_max = arr[i];
                if (s_b_indice)
                    *s_b_indice = i;
            }
        }
        if (s_max == INT_MIN) {
            s_max = -1;
        }
    }

    template<typename Ty>
    void maxAndSecondMax(Ty* arr, unsigned int size, Ty& max, Ty& s_max) {
        max = INT_MIN;
        s_max = INT_MIN;

        for (int i = 0; i < size; ++i) {
            if (arr[i] > max) {
                s_max = max;
                max = arr[i];
            }
            else if (arr[i] > s_max && arr[i] != max) {
                s_max = arr[i];
            }
        }
        if (s_max == INT_MIN) {
            s_max = -1;
        }
    }
}