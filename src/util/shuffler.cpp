#ifndef NUMBERS_SHUFFLER_CPP
#define NUMBERS_SHUFFLER_CPP

#include <algorithm>
#include <ctime>
#include <random>
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
        std::swap(numbers[i], numbers[random_index]);
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


std::vector<unsigned int> NumbersShuffler::durstenfeldShuffle(unsigned int length) {
    std::vector<unsigned int> numbers(length);
    std::iota(numbers.begin(), numbers.end(), 1);

    // Loop from the last element down to the second element
    for (int i = numbers.size() - 1; i > 0; --i) {
        // Generate a random integer j such that 0 <= j <= i
        unsigned int random_index = std::rand() % (i + 1);

        // Swap elements at indices i and random_index
        std::swap(numbers[i], numbers[random_index]);
    }

    return numbers;
}


std::vector<unsigned int> NumbersShuffler::durstenfeldUnifromRandShuffle(unsigned int length) {
    // Create a random number generator seeded with a random device
    std::random_device rd;
    // Create a random number generator.
    // Using a static thread_local engine avoids re-seeding on every function call.
    static thread_local std::mt19937 gen(rd());

    std::vector<unsigned int> numbers(length);
    std::iota(numbers.begin(), numbers.end(), 1);

    // Loop from the last element down to the second element
    for (int i = numbers.size() - 1; i > 0; --i) {
        // Generate a random integer j such that 0 <= j <= i
        std::uniform_int_distribution<> dis(0, i);
        unsigned int random_index = dis(gen);

        // Swap elements at indices i and random_index
        std::swap(numbers[i], numbers[random_index]);
    }

    return numbers;
}

#endif // NUMBERS_SHUFFLER_CPP
