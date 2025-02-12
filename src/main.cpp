#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>

#include "util/statistics.h"
#include "util/shuffler.hpp"

// Struct for keeping algorithm’s name with its function pointer.
struct Algorithm {
    std::string name;
    ShuffleFunc func;
};


// Call the selected shuffling algorithm and display the result.
void generateShuffledList(NumbersShuffler & shuffler,
    ShuffleFunc func,
    const std::string & funcName,
    unsigned int length)
{
    auto perm = (shuffler.*func)(length);
    std::cout << "\nShuffled list (" << length << " elements) using " << funcName << ":\n";

    // If the list is very long, only print the first 20 and last 20 elements.
    if (length > 100) {
        for (unsigned int i = 0; i < 20; ++i)
            std::cout << perm[i] << " ";
        std::cout << "... ";
        for (unsigned int i = length - 20; i < length; ++i)
            std::cout << perm[i] << " ";
        std::cout << "\n";
    } else {
        for (unsigned int i = 0; i < length; ++i)
            std::cout << perm[i] << " ";
        std::cout << "\n";
    }
    std::cout << std::string(80, '=') << "\n\n";
}

void printUsage(const std::string &programName) {
    std::cerr << "Usage:\n"
              << "  " << programName
              << " --mode <generate|stats> --n <permutation length> --algorithm <algorithm name or index> [--iterations <iterations>]\n\n"
              << "Examples:\n"
              << "  " << programName << " --mode generate --n 100 --algorithm 6\n"
              << "  " << programName << " --mode stats --n 1000 --algorithm fisherYatesShuffle --iterations 100\n";
}


int main(int argc, char* argv[])
{
    if (argc < 7) { // minimal number of arguments required
        printUsage(argv[0]);
        return 1;
    }

    std::string mode;
    unsigned int permutationLength = 0;
    unsigned int iterations = 1;
    std::string algorithmArg;
    
    // Argument parsing.
    for (int i = 1; i < argc; ++i)
    {
        std::string arg(argv[i]);
        if (arg == "--mode")
        {
            if (i + 1 < argc)
                mode = argv[++i];
            else {
                std::cerr << "Error: --mode requires an argument.\n";
                printUsage(argv[0]);
                return 1;
            }
        }
        else if (arg == "--n")
        {
            if (i + 1 < argc)
            {
                try {
                    permutationLength = std::stoul(argv[++i]);
                } catch (const std::exception &) {
                    std::cerr << "Error: invalid value for --n.\n";
                    return 1;
                }
            }
            else {
                std::cerr << "Error: --n requires an argument.\n";
                printUsage(argv[0]);
                return 1;
            }
        }
        else if (arg == "--algorithm")
        {
            if (i + 1 < argc)
                algorithmArg = argv[++i];
            else {
                std::cerr << "Error: --algorithm requires an argument.\n";
                printUsage(argv[0]);
                return 1;
            }
        }
        else if (arg == "--iterations")
        {
            if (i + 1 < argc)
            {
                try {
                    iterations = std::stoul(argv[++i]);
                } catch (const std::exception &) {
                    std::cerr << "Error: invalid value for --iterations.\n";
                    return 1;
                }
            }
            else {
                std::cerr << "Error: --iterations requires an argument.\n";
                printUsage(argv[0]);
                return 1;
            }
        }
        else
        {
            std::cerr << "Unknown argument: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }
    
    if (permutationLength == 0)
    {
        std::cerr << "Error: permutation length (--n) must be a positive integer.\n";
        return 1;
    }

    // List of available shuffling algorithms.
    std::vector<Algorithm> algorithms = {
        {"biasedNaiveShuffle",       &NumbersShuffler::biasedNaiveShuffle},
        {"naiveShuffle",             &NumbersShuffler::naiveShuffle},
        {"biasedFisherYatesShuffle", &NumbersShuffler::biasedFisherYatesShuffle},
        {"fisherYatesShuffle",       &NumbersShuffler::fisherYatesShuffle},
        {"biasedDurstenfeldShuffle", &NumbersShuffler::biasedDurstenfeldShuffle},
        {"durstenfeldShuffle",       &NumbersShuffler::durstenfeldShuffle},
        {"randomShuffle",            &NumbersShuffler::randomShuffle},
        {"mergeShuffle",             &NumbersShuffler::mergeShuffle},
        {"parallelMergeShuffle",     &NumbersShuffler::parallelMergeShuffle}
    };

    ShuffleFunc chosenFunc = nullptr;
    std::string chosenName;
    
    // Try interpreting algorithmArg as an index.
    try {
        size_t idx = std::stoul(algorithmArg);
        if (idx < 1 || idx > algorithms.size())
        {
            std::cerr << "Error: algorithm index must be between 1 and " << algorithms.size() << ".\n";
            return 1;
        }
        chosenFunc = algorithms[idx - 1].func;
        chosenName = algorithms[idx - 1].name;
    } catch (const std::exception &) {
        // Not a number; match by name.
        bool found = false;
        for (const auto &algo : algorithms)
        {
            if (algo.name == algorithmArg)
            {
                chosenFunc = algo.func;
                chosenName = algo.name;
                found = true;
                break;
            }
        }
        if (!found)
        {
            std::cerr << "Error: unknown algorithm name: " << algorithmArg << "\n";
            return 1;
        }
    }
    
    NumbersShuffler shuffler;
    
    // Execute the requested mode.
    if (mode == "generate")
    {
        generateShuffledList(shuffler, chosenFunc, chosenName, permutationLength);
    }
    else if (mode == "stats")
    {
        produceShuffleStats(shuffler, chosenFunc, chosenName, permutationLength, iterations);
    }
    else
    {
        std::cerr << "Error: unknown mode: " << mode << "\n";
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}

#endif // MAIN_CPP
