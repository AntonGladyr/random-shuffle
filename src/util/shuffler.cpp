#ifndef NUMBERS_SHUFFLER_CPP
#define NUMBERS_SHUFFLER_CPP

#include <algorithm>
#include <ctime>
#include <random>
#include <numeric>
#include <utility> 

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
        unsigned int randomIndex = std::rand() % length;
        std::swap(numbers[i], numbers[randomIndex]);
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
    // Using a static thread_local engine avoids re-seeding on every function call.
    static thread_local std::mt19937 gen(rd());

    std::vector<unsigned int> numbers(length);
    std::iota(numbers.begin(), numbers.end(), 1);

    for (unsigned int i = 0; i < length; i++) {
        // Generate a random integer
        std::uniform_int_distribution<> dis(0, length - 1);
        unsigned int randomIndex = dis(gen);
        std::swap(numbers[i], numbers[randomIndex]);
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
        unsigned int randomIndex = std::rand() % numbers.size();
        shuffled.push_back(numbers[randomIndex]);
        numbers.erase(numbers.begin() + randomIndex);
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
        unsigned int randomIndex = dis(gen);
        shuffled.push_back(numbers[randomIndex]);
        numbers.erase(numbers.begin() + randomIndex);
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
        unsigned int randomIndex = std::rand() % (i + 1);

        // Swap elements at indices i and randomIndex
        std::swap(numbers[i], numbers[randomIndex]);
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
    // Using a static thread_local engine avoids re-seeding on every function call.
    static thread_local std::mt19937 gen(rd());

    std::vector<unsigned int> numbers(length);
    std::iota(numbers.begin(), numbers.end(), 1);

    // Loop from the last element down to the second element
    for (int i = numbers.size() - 1; i > 0; --i) {
        // Generate a random integer j such that 0 <= j <= i
        std::uniform_int_distribution<> dis(0, i);
        unsigned int randomIndex = dis(gen);

        // Swap elements at indices i and randomIndex
        std::swap(numbers[i], numbers[randomIndex]);
    }

    return numbers;
}


/**
 * @brief Shuffles a sequence of numbers using a sort-based random key pairing approach.
 *
 * This method creates a vector of numbers from 1 to length and assigns each number a random key.
 * The random key for each element is generated using a uniform integer distribution, ensuring that
 * each key is chosen uniformly from the range [0, length - 1]. The method then sorts a temporary vector
 * of pairs (random key, element) based on the random keys. Finally, it reconstructs the original vector
 * in the order determined by the sorted keys, resulting in a shuffled sequence.
 *
 * @note This approach involves sorting the temporary vector, which has a time complexity of O(n log n).
 *       Although the randomness is generally good thanks to std::mt19937 and std::random_device, the method
 *       may be less efficient than an in-place shuffle like the Fisher-Yates algorithm.
 *
 * @param length The number of elements to shuffle.
 * @return A vector containing the numbers 1 to length in a pseudo-random order.
 */
std::vector<unsigned int> NumbersShuffler::randomShuffle(unsigned int length) {
    // Create a random number generator seeded with a random device
    std::random_device rd;
    // Using a static thread_local engine avoids re-seeding on every function call.
    static thread_local std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dis(0, std::numeric_limits<unsigned int>::max());

    std::vector<unsigned int> numbers(length);
    std::iota(numbers.begin(), numbers.end(), 1);

    // Create a temporary vector of pairs: each pair contains a random key and an element.
    std::vector<std::pair<unsigned int, unsigned int>> paired;
    paired.reserve(length);

    // Pair each element with a random key.
    for (unsigned int i = 0; i < length; ++i) {
        paired.emplace_back(dis(gen), numbers[i]);
    }

    // Sort the vector of pairs based on the random key.
    // Use a stable sort to preserve the random ordering of equal keys.
    std::stable_sort(paired.begin(), paired.end(),
        [](const std::pair<unsigned int, unsigned int>& a, const std::pair<unsigned int, unsigned int>& b) {
            return a.first < b.first;
        }
    );

    for (unsigned int i = 0; i < length; ++i) {
        numbers[i] = paired[i].second;
    }

    return numbers;
}


// Helper function for recursive merge shuffling.
// For small subranges (fewer than 'threshold' elements), it falls back to a simple Fisher–Yates shuffle.
static void mergeShuffleRec(
    std::vector<unsigned int>& arr,
    unsigned int start,
    unsigned int end,
    std::mt19937 &gen
) {
    unsigned int n = end - start;

    if (n <= 1) {
        return;
    }

    // Threshold to fall back to Durstenfeld algorithm.
    const unsigned int threshold = 16;
    if (n < threshold) {
        for (unsigned int i = start; i < end; i++) {
            // Choose a random index in the range [i, end - 1]
            std::uniform_int_distribution<unsigned int> dis(i, end - 1);
            unsigned int randomIndex = dis(gen);
            std::swap(arr[i], arr[randomIndex]);
        }

        return;
    }

    // Recursively split the range into two halves and shuffle each half.
    unsigned int mid = start + n / 2;
    mergeShuffleRec(arr, start, mid, gen);
    mergeShuffleRec(arr, mid, end, gen);

    // Merge the two shuffled halves.
    std::vector<unsigned int> temp;
    temp.reserve(n);

    unsigned int left = start;
    unsigned int right = mid;

    while (left < mid && right < end) {
        unsigned int leftCount = mid - left;
        unsigned int rightCount = end - right;
        std::uniform_int_distribution<unsigned int> dis(0, leftCount + rightCount - 1);
        unsigned int pick = dis(gen);

        if (pick < leftCount) {
            temp.push_back(arr[left]);
            left++;
        } else {
            temp.push_back(arr[right]);
            right++;
        }
    }

    // Append any remaining elements from the left or right half.

    while (left < mid) {
        temp.push_back(arr[left]);
        left++;
    }

    while (right < end) {
        temp.push_back(arr[right]);
        right++;
    }

    // The merged (shuffled) result.
    for (unsigned int i = 0; i < n; i++) {
        arr[start + i] = temp[i];
    }
}

/**
 * @brief Shuffles a sequence using the merge shuffle algorithm.
 *
 * This function creates a vector containing numbers from 1 to length, and then
 * recursively shuffles it by dividing the vector into halves, shuffling each half,
 * and merging the two halves together via a uniformly random interleaving.
 *
 * @param length The number of elements to shuffle.
 * @return A vector containing the numbers 1 to length in a pseudo-random order.
 */
std::vector<unsigned int> NumbersShuffler::mergeShuffle(unsigned int length) {
    // Create a random number generator seeded with a random device.
    std::random_device rd;
    // Using a static thread_local engine avoids re-seeding on every function call.
    static thread_local std::mt19937 gen(rd());

    std::vector<unsigned int> numbers(length);
    std::iota(numbers.begin(), numbers.end(), 1);

    mergeShuffleRec(numbers, 0, numbers.size(), gen);
    return numbers;
}

#endif // NUMBERS_SHUFFLER_CPP
