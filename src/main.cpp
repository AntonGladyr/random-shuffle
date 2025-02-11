#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>


#include "util/shuffler.hpp"

// A type alias for a pointer-to-member function
using ShuffleFunc = std::vector<unsigned int> (NumbersShuffler::*)(unsigned int);

/**
 * @brief Runs the given shuffle method many times and collects per-position frequency statistics.
 *
 * @param shuffler An instance of NumbersShuffler.
 * @param func A pointer to the shuffle function to test.
 * @param funcName The name of the function.
 * @param length The length of the permutation (i.e. numbers 1..length).
 * @param iterations How many shuffles to perform.
 */
void testShuffleFunction(
    NumbersShuffler& shuffler,
    ShuffleFunc func,
    const std::string& funcName,
    unsigned int length,
    unsigned int iterations
) {
    // A table of frequencies where frequencies[i][j] counts how many times the number j appears at position i.
    std::vector<std::vector<unsigned int>> frequencies(
        length,
        std::vector<unsigned int>(length + 1, 0)
    );

    for (unsigned int iter = 0; iter < iterations; ++iter) {
        std::vector<unsigned int> perm = (shuffler.*func)(length);

        // Record frequency for each position.
        for (unsigned int pos = 0; pos < length; ++pos) {
            unsigned int number = perm[pos];
            frequencies[pos][number]++;
        }
    }

    std::cout << "Statistics for " << funcName << " ("
        << iterations << " iterations, permutation length = " << length << "):\n";

    for (unsigned int pos = 0; pos < length; ++pos) {
        std::cout << "  Position " << std::setw(2) << (pos + 1) << ": ";

        for (unsigned int number = 1; number <= length; ++number) {
            std::cout << number << " -> " << std::setw(8) << frequencies[pos][number] << "  ";
        }

        std::cout << "\n";
    }

    std::cout << std::string(80, '-') << "\n";
}


int main()
{
    NumbersShuffler shuffler;

    const unsigned int permutationLength = 5;
    const unsigned int iterations = 888888;

    testShuffleFunction(
        shuffler, &NumbersShuffler::biasedNaiveShuffle, "biasedNaiveShuffle", permutationLength, iterations
    );
    testShuffleFunction(
        shuffler, &NumbersShuffler::naiveShuffle, "naiveShuffle", permutationLength, iterations
    );
    testShuffleFunction(
        shuffler, &NumbersShuffler::biasedFisherYatesShuffle, "biasedFisherYatesShuffle", permutationLength, iterations
    );
    testShuffleFunction(
        shuffler, &NumbersShuffler::fisherYatesShuffle, "fisherYatesShuffle", permutationLength, iterations
    );
    testShuffleFunction(
        shuffler, &NumbersShuffler::biasedDurstenfeldShuffle, "biasedDurstenfeldShuffle", permutationLength, iterations
    );
    testShuffleFunction(
        shuffler, &NumbersShuffler::durstenfeldShuffle, "durstenfeldShuffle", permutationLength, iterations
    );
    testShuffleFunction(
        shuffler, &NumbersShuffler::randomShuffle, "randomShuffle", permutationLength, iterations
    );

    return 0;
}

#endif // MAIN_CPP
