#ifndef LIST_SHUFFLER_CPP
#define LIST_SHUFFLER_CPP

#include <algorithm>
#include <ctime>
#include <numeric>

#include "util/shuffler.hpp"

NumbersShuffler::NumbersShuffler() {
    std::srand(std::time({})); // use current time as seed for random generator
}

std::vector<unsigned int> NumbersShuffler::naiveShuffle(unsigned int length) {
    std::vector<unsigned int> numbers(length);
    std::iota(numbers.begin(), numbers.end(), 1);

    for (unsigned int i = 0; i < length; i++) {
        unsigned int random_index = std::rand() % length;
        std::iter_swap(numbers.begin() + random_index, numbers.begin() + i);
    }

    return numbers;
}

std::vector<unsigned int> NumbersShuffler::fisherYatesShuffle(unsigned int length) {
    std::vector<unsigned int> numbers(length);
    std::vector<unsigned int> shuffled;
    std::iota(numbers.begin(), numbers.end(), 1);

    // While there are still numbers left, pick a random index,
    // move the element at that index to the shuffled vector,
    // and remove it from the original vector.
    while (!numbers.empty()) {
        unsigned int random_index = std::rand() % numbers.size();
        shuffled.push_back(numbers[random_index]);
        numbers.erase(numbers.begin() + random_index);
    }

    return shuffled;
}


#endif // LIST_SHUFFLER_CPP
