#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>

#include "util/shuffler.hpp"

int main()
{
    NumbersShuffler shuffler = NumbersShuffler();

    for (int i = 0; i <= 128; i++) {
        std::vector<unsigned int> shuffledNumbers = shuffler.durstenfeldUnifromRandShuffle(4);
        for (unsigned int num: shuffledNumbers) {
            std::cout << num;
        }
        std::cout << std::endl;
    }
}

#endif // MAIN_CPP
