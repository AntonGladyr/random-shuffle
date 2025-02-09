#ifndef LIST_SHUFFLER_CPP
#define LIST_SHUFFLER_CPP

#include <algorithm>
#include <ctime>
#include <numeric>

#include "helpers/shuffler.hpp"

ListShuffler::ListShuffler() {
    std::srand(std::time({})); // use current time as seed for random generator
}

std::vector<unsigned int> ListShuffler::naiveShuffle(unsigned int length) {
    std::vector<unsigned int> numbers(length);
    std::iota(numbers.begin(), numbers.end(), 1);

    for (unsigned int i = 0; i < length; i++) {
        unsigned int random_index = std::rand() % length;
        std::iter_swap(numbers.begin() + random_index, numbers.begin() + i);
    }

    return numbers;
}

#endif
