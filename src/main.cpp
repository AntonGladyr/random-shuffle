#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "util/command_line.hpp"
#include "util/helpers.hpp"
#include "util/statistics.hpp"
#include "util/shuffler.hpp"


int main(int argc, char* argv[]) {
    try {
        ProgramOptions options = parseArguments(argc, argv);
        std::pair<std::string, ShuffleFunc> algorithmPair = selectAlgorithm(options.algorithm);
        std::string chosenName = algorithmPair.first;
        ShuffleFunc chosenFunc = algorithmPair.second;

        NumbersShuffler shuffler;

        if (options.mode == "generate") {
            generateShuffledList(shuffler, chosenFunc, chosenName, options.permutationLength);
        } else if (options.mode == "stats") {
            produceShuffleStats(
                shuffler, chosenFunc, chosenName, options.permutationLength, options.iterations
            );
        } else {
            throw std::runtime_error("Error: unknown mode: " + options.mode);
        }
    }
    catch (const std::exception &ex) {
        std::cerr << ex.what() << "\n";
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}


#endif // MAIN_CPP
