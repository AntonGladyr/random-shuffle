#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdlib>

#include "util/command_line.hpp"

void printUsage(const std::string &programName) {
    std::cerr << "Usage:\n"
              << "  " << programName
              << " --mode <generate|stats> --n <permutation length> --algorithm <algorithm name or index> [--iterations <iterations>]\n\n"
              << "Examples:\n"
              << "  " << programName << " --mode generate --n 100 --algorithm 6\n"
              << "  " << programName << " --mode stats --n 1000 --algorithm fisherYatesShuffle --iterations 100\n";
}

ProgramOptions parseArguments(int argc, char* argv[]) {
    ProgramOptions options;
    options.iterations = 1;

    if (argc < 7) {
        throw std::runtime_error("Insufficient arguments provided.");
    }
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "--mode") {
            if (i + 1 < argc) {
                options.mode = argv[++i];
            } else {
                throw std::runtime_error("Error: --mode requires an argument.");
            }
        }
        else if (arg == "--n") {
            if (i + 1 < argc) {
                try {
                    options.permutationLength = static_cast<unsigned int>(std::stoul(argv[++i]));
                } catch (const std::exception &) {
                    throw std::runtime_error("Error: invalid value for --n.");
                }
            } else {
                throw std::runtime_error("Error: --n requires an argument.");
            }
        }
        else if (arg == "--algorithm") {
            if (i + 1 < argc) {
                options.algorithm = argv[++i];
            } else {
                throw std::runtime_error("Error: --algorithm requires an argument.");
            }
        }
        else if (arg == "--iterations") {
            if (i + 1 < argc) {
                try {
                    options.iterations = static_cast<unsigned int>(std::stoul(argv[++i]));
                } catch (const std::exception &) {
                    throw std::runtime_error("Error: invalid value for --iterations.");
                }
            } else {
                throw std::runtime_error("Error: --iterations requires an argument.");
            }
        }
        else {
            throw std::runtime_error("Unknown argument: " + arg);
        }
    }
    if (options.permutationLength == 0) {
        throw std::runtime_error("Error: permutation length (--n) must be a positive integer.");
    }
    return options;
}
