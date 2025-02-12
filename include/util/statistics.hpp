#ifndef STATISTICS_H
#define STATISTICS_H

#include <vector>
#include <string>

#include "util/helpers.hpp"
#include "util/shuffler.hpp"


/**
 * @brief Runs the given shuffle method many times and collects per-position frequency statistics.
 *
 * @param shuffler An instance of NumbersShuffler.
 * @param func A pointer to the shuffle function to test.
 * @param funcName The name of the function.
 * @param length The length of the permutation (i.e. numbers 1..length).
 * @param iterations How many shuffles to perform.
 */
void produceShuffleStats(
    NumbersShuffler& shuffler,
    ShuffleFunc func,
    const std::string& funcName,
    unsigned int length,
    unsigned int iterations
);

#endif // STATISTICS_H
