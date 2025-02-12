#ifndef COMMAND_LINE_HPP
#define COMMAND_LINE_HPP

#include <string>

// Holds the parsed command-line options.
struct ProgramOptions {
    std::string mode;
    unsigned int permutationLength;
    unsigned int iterations;
    std::string algorithm;
};

// Prints the usage information.
void printUsage(const std::string &programName);

// Parses the command-line arguments.
// Throws a std::runtime_error if parsing fails.
ProgramOptions parseArguments(int argc, char* argv[]);

#endif // COMMAND_LINE_HPP
