#ifndef MAIN_CPP
#define MAIN_CPP

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


#include "util/shuffler.hpp"

// A type alias for a pointer-to-member function
using ShuffleFunc = std::vector<unsigned int> (NumbersShuffler::*)(unsigned int) const;

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

    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

    for (unsigned int iter = 0; iter < iterations; ++iter) {
        std::vector<unsigned int> perm = (shuffler.*func)(length);

        // Record frequency for each position.
        for (unsigned int pos = 0; pos < length; ++pos) {
            unsigned int number = perm[pos];
            frequencies[pos][number]++;
        }
    }

    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds::rep elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - startTime
    ).count();

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

    // Additional statistics.
    double expected = static_cast<double>(iterations) / length;
    std::cout << "Additional Statistics per Position:\n";
    std::cout << std::setw(10) << "Position"
              << std::setw(15) << "Chi-Square"
              << std::setw(15) << "Mean"
              << std::setw(15) << "StdDev"
              << std::setw(15) << "Entropy" << "\n";

    for (unsigned int pos = 0; pos < length; ++pos) {
        double chiSquare = 0.0;
        double mean = 0.0;
        double entropy = 0.0;
        // Compute chi-square statistic and the weighted sum for mean.
        for (unsigned int number = 1; number <= length; ++number) {
            double observed = frequencies[pos][number];
            chiSquare += ((observed - expected) * (observed - expected)) / expected;
            mean += number * observed;
            double p = observed / static_cast<double>(iterations);
            if (p > 0.0) {
                entropy += -p * std::log(p);  // using natural logarithm
            }
        }
        mean /= iterations;
        
        // Compute variance for standard deviation.
        double variance = 0.0;
        for (unsigned int number = 1; number <= length; ++number) {
            double observed = frequencies[pos][number];
            variance += ((number - mean) * (number - mean)) * observed;
        }
        variance /= iterations;
        double stddev = std::sqrt(variance);

        std::cout << std::setw(10) << (pos + 1)
                  << std::setw(15) << std::fixed << std::setprecision(2) << chiSquare
                  << std::setw(15) << mean
                  << std::setw(15) << stddev
                  << std::setw(15) << entropy << "\n";
    }

    std::cout << "\nExecution Time for " << funcName << ": "
        << elapsedTime << " ms" << "\n";

    std::cout << std::string(80, '=') << "\n\n";
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
    testShuffleFunction(
        shuffler, &NumbersShuffler::mergeShuffle, "mergeShuffle", permutationLength, iterations
    );
    testShuffleFunction(
        shuffler, &NumbersShuffler::parallelMergeShuffle, "parallelMergeShuffle", permutationLength, iterations
    );

    return 0;
}

#endif // MAIN_CPP
