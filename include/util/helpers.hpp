#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>
#include <vector>
#include <utility>
#include "util/shuffler.hpp"


// A pointer‐to‐member function type for a NumbersShuffler shuffle method.
using ShuffleFunc = std::vector<unsigned int> (NumbersShuffler::*)(unsigned int) const;

// Structure for pairing an algorithm’s name with its function pointer.
struct Algorithm {
    std::string name;
    ShuffleFunc func;
};

// Returns a pair (algorithm name, function pointer) based on the provided argument.
// Throws std::runtime_error if the algorithm cannot be found.
std::pair<std::string, ShuffleFunc> selectAlgorithm(const std::string &algorithmArg);

// Prints the permutation.
void printPermutation(const std::vector<unsigned int> &perm, const std::string &funcName);

// Calls the shuffling algorithm and prints the shuffled list.
void generateShuffledList(
    NumbersShuffler &shuffler,
    ShuffleFunc func,
    const std::string &funcName,
    unsigned int length
);

#endif // HELPERS_HPP
