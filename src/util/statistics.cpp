#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "util/statistics.hpp"


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
) {
    std::cout << "Testing " << funcName
              << " with permutation length = " << length
              << " and iterations = " << iterations << "\n";

    // For one iteration only time the shuffle and show a small sample.
    if (iterations <= 1) {
        std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
        std::vector<unsigned int> perm = (shuffler.*func)(length);
        std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
        long long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

        std::cout << "\nExecution Time for " << funcName << ": " 
                  << elapsedTime << " ms\n";
        std::cout << "Sample output (first 10 numbers): ";
        unsigned int sampleCount = (length < 10 ? length : 10);
        for (unsigned int i = 0; i < sampleCount; ++i) {
            std::cout << perm[i] << " ";
        }
        std::cout << "\n" << std::string(80, '=') << "\n\n";
        return;
    }

    // Frequencies are stored in a 1D vector where the frequency at (pos, number)
    // is located at: index = pos * (length + 1) + number.
    unsigned int frequencySize = length * (length + 1);
    std::vector<unsigned int> frequencies(frequencySize, 0);

    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    for (unsigned int iter = 0; iter < iterations; ++iter) {
        std::vector<unsigned int> perm = (shuffler.*func)(length);
        for (unsigned int pos = 0; pos < length; ++pos) {
            unsigned int number = perm[pos];
            unsigned int index = pos * (length + 1) + number;
            frequencies[index] += 1;
        }
    }
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    long long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "\nStatistics for " << funcName
              << " (" << iterations << " iterations, permutation length = " << length << "):\n";

    // For small permutations, show the full frequency distribution per position.
    if (length <= 5) {
        for (unsigned int pos = 0; pos < length; ++pos) {
            std::cout << "  Position " << std::setw(2) << (pos + 1) << ": ";
            for (unsigned int number = 1; number <= length; ++number) {
                unsigned int freq = frequencies[pos * (length + 1) + number];
                std::cout << number << " -> " << std::setw(8) << freq << "  ";
            }
            std::cout << "\n";
        }
    } else {
        // Show only the overall min, max, and average frequency.
        unsigned int overallMinFreq = frequencies[0 * (length + 1) + 1];
        unsigned int overallMaxFreq = overallMinFreq;
        unsigned int totalFrequency = 0;
        for (unsigned int pos = 0; pos < length; ++pos) {
            for (unsigned int number = 1; number <= length; ++number) {
                unsigned int freq = frequencies[pos * (length + 1) + number];
                totalFrequency += freq;
                if (freq < overallMinFreq) {
                    overallMinFreq = freq;
                }
                if (freq > overallMaxFreq) {
                    overallMaxFreq = freq;
                }
            }
        }
        double averageFreq = static_cast<double>(totalFrequency) / (length * length);
        std::cout << "Frequency Summary: min frequency = " << overallMinFreq
                  << ", max frequency = " << overallMaxFreq
                  << ", average frequency = " << std::fixed << std::setprecision(2) << averageFreq << "\n";
    }

    // Additional statistics (e.g., Chi-Square, Mean, Standard Deviation, Entropy).
    std::vector<double> chiSquares(length, 0.0);
    std::vector<double> means(length, 0.0);
    std::vector<double> stddevs(length, 0.0);
    std::vector<double> entropies(length, 0.0);

    double expected = static_cast<double>(iterations) / static_cast<double>(length);
    for (unsigned int pos = 0; pos < length; ++pos) {
        double chiSquare = 0.0;
        double mean = 0.0;
        double entropy = 0.0;
        unsigned int number = 1;
        for (number = 1; number <= length; ++number) {
            unsigned int freq = frequencies[pos * (length + 1) + number];
            double observed = static_cast<double>(freq);
            chiSquare += ((observed - expected) * (observed - expected)) / expected;
            mean += number * observed;
            double probability = observed / static_cast<double>(iterations);
            if (probability > 0.0) {
                entropy += -probability * std::log(probability);
            }
        }
        mean /= static_cast<double>(iterations);
        double variance = 0.0;
        for (number = 1; number <= length; ++number) {
            unsigned int freq = frequencies[pos * (length + 1) + number];
            double observed = static_cast<double>(freq);
            variance += ((number - mean) * (number - mean)) * observed;
        }
        variance /= static_cast<double>(iterations);
        double stddev = std::sqrt(variance);

        chiSquares[pos] = chiSquare;
        means[pos] = mean;
        stddevs[pos] = stddev;
        entropies[pos] = entropy;
    }

    if (length <= 5) {
        std::cout << "\nAdditional Statistics per Position:\n";
        std::cout << std::setw(10) << "Position"
                  << std::setw(15) << "Chi-Square"
                  << std::setw(15) << "Mean"
                  << std::setw(15) << "StdDev"
                  << std::setw(15) << "Entropy" << "\n";
        for (unsigned int pos = 0; pos < length; ++pos) {
            std::cout << std::setw(10) << (pos + 1)
                      << std::setw(15) << std::fixed << std::setprecision(2) << chiSquares[pos]
                      << std::setw(15) << means[pos]
                      << std::setw(15) << stddevs[pos]
                      << std::setw(15) << entropies[pos] << "\n";
        }
    } else {
        // For large permutations aggregate the statistics across positions.
        double minChiSquare = chiSquares[0];
        double maxChiSquare = chiSquares[0];
        double minMean = means[0];
        double maxMean = means[0];
        double minStdDev = stddevs[0];
        double maxStdDev = stddevs[0];
        double minEntropy = entropies[0];
        double maxEntropy = entropies[0];
        for (unsigned int pos = 1; pos < length; ++pos) {
            if (chiSquares[pos] < minChiSquare) {
                minChiSquare = chiSquares[pos];
            }
            if (chiSquares[pos] > maxChiSquare) {
                maxChiSquare = chiSquares[pos];
            }
            if (means[pos] < minMean) {
                minMean = means[pos];
            }
            if (means[pos] > maxMean) {
                maxMean = means[pos];
            }
            if (stddevs[pos] < minStdDev) {
                minStdDev = stddevs[pos];
            }
            if (stddevs[pos] > maxStdDev) {
                maxStdDev = stddevs[pos];
            }
            if (entropies[pos] < minEntropy) {
                minEntropy = entropies[pos];
            }
            if (entropies[pos] > maxEntropy) {
                maxEntropy = entropies[pos];
            }
        }
        std::cout << "\nAdditional Statistics Summary:\n";
        std::cout << std::setw(15) << "Statistic"
                  << std::setw(20) << "Min Value"
                  << std::setw(20) << "Max Value" << "\n";
        std::cout << std::setw(15) << "Chi-Square"
                  << std::setw(20) << std::fixed << std::setprecision(2) << minChiSquare
                  << std::setw(20) << maxChiSquare << "\n";
        std::cout << std::setw(15) << "Mean"
                  << std::setw(20) << minMean
                  << std::setw(20) << maxMean << "\n";
        std::cout << std::setw(15) << "StdDev"
                  << std::setw(20) << minStdDev
                  << std::setw(20) << maxStdDev << "\n";
        std::cout << std::setw(15) << "Entropy"
                  << std::setw(20) << minEntropy
                  << std::setw(20) << maxEntropy << "\n";
    }

    std::cout << "\nExecution Time for " << funcName << ": "
              << elapsedTime << " ms\n";
    std::cout << std::string(80, '=') << "\n\n";
}
