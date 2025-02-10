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


/**
 * @brief Generates a biased pseudo-random permutation of integers from 1 to length.
 *
 * This function creates a vector containing the numbers 1 through `length` and then
 * performs a "naive" shuffle by iterating over each element and swapping it with an
 * element at a randomly chosen index. The random index is computed using:
 *
 *     std::rand() % length
 *
 * **Important Caveats:**
 *
 * 1. **Naive Shuffle Bias:**  
 *    This shuffle algorithm is naive because it swaps each element with any other
 *    element in the entire range, regardless of whether it has already been shuffled.
 *    In contrast, the unbiased Fisher–Yates shuffle restricts the random index to the
 *    unshuffled portion (i.e., using `std::rand() % (length - i)`). The current approach
 *    leads to some permutations being more likely than others.
 *
 * 2. **Modulo Bias:**  
 *    Using the modulo operator (`%`) to reduce the range of `std::rand()` can introduce
 *    bias if the range of `std::rand()` (typically 0 to RAND_MAX) is not an exact multiple
 *    of `length`.  
 *    For example, if RAND_MAX is 32767 and `length` is 100, then there are 32768 possible
 *    outcomes from `std::rand()`. Since 32768 divided by 100 gives a quotient of 327 with a
 *    remainder of 68, the remainders 0 through 67 will occur 328 times each, while 68 through 99
 *    will occur only 327 times each, resulting in a biased distribution.
 *
 * 3. **Quality of std::rand():**  
 *    The underlying random number generator, `std::rand()`, is generally considered low-quality.
 *    It is often implemented as a simple linear congruential generator, which may exhibit patterns
 *    or low entropy, further compounding the overall bias of the shuffle.
 *
 * @param length The number of elements in the sequence to be shuffled.
 * @return A vector of unsigned integers from 1 to `length` in a pseudo-random order.
 */
std::vector<unsigned int> NumbersShuffler::biasedNaiveShuffle(unsigned int length) {
    std::vector<unsigned int> numbers(length);
    std::iota(numbers.begin(), numbers.end(), 1);

    for (unsigned int i = 0; i < length; i++) {
        unsigned int random_index = std::rand() % length;
        std::swap(numbers[i], numbers[random_index]);
    }

    return numbers;
}


/**
 * @brief Generates a pseudo-random permutation of integers from 1 to length.
 *
 * This function creates a vector containing the numbers 1 through `length` and then
 * performs a "naive" shuffle by iterating over each element and swapping it with an
 * element at a randomly chosen index. The random index is computed using Mersenne Twister 
 * pseudorandom number generator (a.k.a mt19937).
 *
 * @param length The number of elements in the sequence to be shuffled.
 * @return A vector of unsigned integers from 1 to `length` in a pseudo-random order.
 */
std::vector<unsigned int> NumbersShuffler::naiveShuffle(unsigned int length) {
    // Create a random number generator seeded with a random device
    std::random_device rd;
    // Create a random number generator.
    // Using a static thread_local engine avoids re-seeding on every function call.
    static thread_local std::mt19937 gen(rd());

    std::vector<unsigned int> numbers(length);
    std::iota(numbers.begin(), numbers.end(), 1);

    for (unsigned int i = 0; i < length; i++) {
        // unsigned int random_index = std::rand() % length;
        // Generate a random integer
        std::uniform_int_distribution<> dis(0, i);
        unsigned int random_index = dis(gen);
        std::swap(numbers[i], numbers[random_index]);
    }

    return numbers;
}


/**
 * @brief Shuffles a sequence in a biased way using a variant of the Fisher–Yates algorithm.
 *
 * This implementation creates a vector containing numbers from 1 to length,
 * then repeatedly selects a random element from the remaining numbers, moves it to
 * a new vector, and erases it from the original vector. This process continues until
 * all elements have been moved to the shuffled vector.
 *
 * @note This approach is less efficient since removing
 *       an element from a vector has O(n) complexity.
 * 
 * @note This method uses std::rand() with the modulo operator to generate random indices,
 *       which can introduce modulo bias if RAND_MAX is not a multiple of the range.
 *
 * @param length The number of elements to shuffle.
 * @return A vector containing the shuffled sequence.
 */
std::vector<unsigned int> NumbersShuffler::biasedFisherYatesShuffle(unsigned int length) {
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


/**
 * @brief Shuffles a sequence using a variant of the Fisher–Yates algorithm.
 *
 * This implementation creates a vector containing numbers from 1 to length,
 * then repeatedly selects a random element from the remaining numbers, moves it to
 * a new vector, and erases it from the original vector. This process continues until
 * all elements have been moved to the shuffled vector.
 *
 * @note This approach is less efficient since removing
 *       an element from a vector has O(n) complexity.
 *
 * @param length The number of elements to shuffle.
 * @return A vector containing the shuffled sequence.
 */
std::vector<unsigned int> NumbersShuffler::fisherYatesShuffle(unsigned int length) {
    // Create a random number generator seeded with a random device
    std::random_device rd;
    // Create a random number generator.
    // Using a static thread_local engine avoids re-seeding on every function call.
    static thread_local std::mt19937 gen(rd());

    std::vector<unsigned int> numbers(length);
    std::vector<unsigned int> shuffled;
    std::iota(numbers.begin(), numbers.end(), 1);

    // While there are still numbers left, pick a random index,
    // move the element at that index to the shuffled vector,
    // and remove it from the original vector.
    while (!numbers.empty()) {
        std::uniform_int_distribution<> dis(0, numbers.size() - 1);
        unsigned int random_index = dis(gen);
        shuffled.push_back(numbers[random_index]);
        numbers.erase(numbers.begin() + random_index);
    }

    return shuffled;
}


/**
 * @brief Shuffles a sequence using a biased version of the Durstenfeld algorithm.
 *
 * This function shuffles a vector of numbers from 1 to length by iterating backwards
 * and swapping each element with an element at a random index in the range [0, i]. However,
 * it uses std::rand() with the modulo operator to generate random indices, which can introduce
 * modulo bias if RAND_MAX is not a multiple of the range.
 *
 * @param length The number of elements to shuffle.
 * @return A vector containing the shuffled sequence.
 */
std::vector<unsigned int> NumbersShuffler::biasedDurstenfeldShuffle(unsigned int length) {
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


/**
 * @brief Shuffles a sequence using the unbiased Durstenfeld shuffle algorithm.
 *
 * This function implements the Durstenfeld algorithm using a modern random number generator
 * (std::mt19937) with a uniform distribution to produce unbiased random indices. It iterates
 * backwards through the vector, swapping each element with another element at a randomly
 * chosen index from the unshuffled portion of the vector.
 *
 * @param length The number of elements to shuffle.
 * @return A vector containing the shuffled sequence.
 */
std::vector<unsigned int> NumbersShuffler::durstenfeldShuffle(unsigned int length) {
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
