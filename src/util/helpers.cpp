#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>

#include "util/helpers.hpp"


const unsigned int kMaxElementsToShow = 20;
const unsigned int kThresholdForTruncatedOutput = 100;


std::pair<std::string, ShuffleFunc> selectAlgorithm(const std::string &algorithmArg) {
    std::vector<Algorithm> algorithms;
    algorithms.push_back({"biasedNaiveShuffle",       &NumbersShuffler::biasedNaiveShuffle});
    algorithms.push_back({"naiveShuffle",             &NumbersShuffler::naiveShuffle});
    algorithms.push_back({"biasedFisherYatesShuffle", &NumbersShuffler::biasedFisherYatesShuffle});
    algorithms.push_back({"fisherYatesShuffle",       &NumbersShuffler::fisherYatesShuffle});
    algorithms.push_back({"biasedDurstenfeldShuffle", &NumbersShuffler::biasedDurstenfeldShuffle});
    algorithms.push_back({"durstenfeldShuffle",       &NumbersShuffler::durstenfeldShuffle});
    algorithms.push_back({"randomShuffle",            &NumbersShuffler::randomShuffle});
    algorithms.push_back({"mergeShuffle",             &NumbersShuffler::mergeShuffle});
    algorithms.push_back({"parallelMergeShuffle",     &NumbersShuffler::parallelMergeShuffle});

    unsigned int count = static_cast<unsigned int>(algorithms.size());
    try {
        unsigned long index = std::stoul(algorithmArg);
        if (index < 1 || index > count) {
            throw std::runtime_error(
                "Error: algorithm index must be between 1 and " + std::to_string(count) + "."
            );
        }
        return std::pair<std::string, ShuffleFunc>(
            algorithms[static_cast<unsigned int>(index - 1)].name,
            algorithms[static_cast<unsigned int>(index - 1)].func);
    } catch (const std::exception &) {
        for (unsigned int i = 0; i < count; i++) {
            if (algorithms[i].name == algorithmArg) {
                return std::pair<std::string, ShuffleFunc>(algorithms[i].name, algorithms[i].func);
            }
        }
        throw std::runtime_error("Error: unknown algorithm name: " + algorithmArg);
    }
}


void printPermutation(const std::vector<unsigned int> &perm, const std::string &funcName) {
    unsigned int length = static_cast<unsigned int>(perm.size());
    std::cout << "\nShuffled list (" << length << " elements) using " << funcName << ":\n";

    if (length > kThresholdForTruncatedOutput) {
        for (unsigned int i = 0; i < kMaxElementsToShow; i++) {
            std::cout << perm[i] << " ";
        }
        std::cout << "... ";
        for (unsigned int i = length - kMaxElementsToShow; i < length; i++) {
            std::cout << perm[i] << " ";
        }
        std::cout << "\n";
    } else {
        for (unsigned int i = 0; i < length; i++) {
            std::cout << perm[i] << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::string(80, '=') << "\n\n";
}


void generateShuffledList(
    NumbersShuffler &shuffler,
    ShuffleFunc func,
    const std::string &funcName,
    unsigned int length
) {
    std::vector<unsigned int> perm = (shuffler.*func)(length);
    printPermutation(perm, funcName);
}
