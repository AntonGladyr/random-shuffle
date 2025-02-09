#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>

#include "helpers/shuffler.hpp"

int main()
{
    ListShuffler shuffler = ListShuffler();

    for (int i = 0; i <= 30; i++) {
        std::vector<unsigned int> shuffledNumbers = shuffler.naiveShuffle(3);
        for (unsigned int num: shuffledNumbers) {
            std::cout << num;
        }
        std::cout << std::endl;
    }
}

#endif
